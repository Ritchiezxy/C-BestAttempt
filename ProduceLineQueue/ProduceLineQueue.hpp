#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>

template<typename T>
class ProdLineQueue
{
public:
	/*!
	 * @brief constructor
	 */ 
	ProdLineQueue();
	/*!
	 * @brief destructor
	 */ 
   ~ProdLineQueue();
   /*!
    * @brief popping out some values from queue when the queue store some values.
	*/ 
	T pop();
	/*!
     * @brief pushing some values into queue when the length of the queue was less than maximum size.
	 */ 
	void push(T &value);
	/*!
	 * @brief setting the maximum size of the queue
	 */
	void setMaxSize(typename std::queue<T>::size_type size);
private:
	std::queue<T> _queue;
	/*!
     * @brief locker
     */
    std::mutex _mutex;
 
	/*!
     * @brief condition that is used to send notification to producer 
	 *        thread production line was free.
     */
    std::condition_variable _prodLineFree;
 
    /*!
     * @brief condition that is used to send notification to customer thread production was 
	 *        produced.
     */
    std::condition_variable _prodProduced;
    /*!
	 * @breif the maximun size of the queue that is used to limit the queue length. 
	 */ 
    typename std::queue<T>::size_type _queueMaxSize = typename std::queue<T>::size_type(-1);
}; 

template<typename T>
ProdLineQueue<T>::ProdLineQueue()
{	
}

template<typename T>
ProdLineQueue<T>::~ProdLineQueue()
{
}
 
template<typename T>
T ProdLineQueue<T>::pop()
{
	T value;
	
    std::unique_lock<std::mutex> uniLock(_mutex);  
    while (_queue.empty())
    {
		_prodProduced.wait(uniLock);
    }
    
    value = _queue.front();
    _queue.pop();
    
    _prodLineFree.notify_all();
    uniLock.unlock();
    
    return value;
}

template<typename T>
void ProdLineQueue<T>::push(T &value)
{
	std::unique_lock<std::mutex> uniLock(_mutex);
	
	while (_queue.size() == _queueMaxSize)
	{
		_prodLineFree.wait(uniLock);
	}
	_queue.push(value);

    _prodProduced.notify_all();
    uniLock.unlock();
}

template<typename T>
void ProdLineQueue<T>::setMaxSize(typename std::queue<T>::size_type size)
{
	_queueMaxSize = size; 
}

