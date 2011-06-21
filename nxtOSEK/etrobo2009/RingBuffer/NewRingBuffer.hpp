#ifndef RINGBUFFER_H_
#define RINGBUFFER_H_

/**
 * Ring Buffer Data Structure.
 */
template <class T>
class NewRingBuffer
{
public:
    /**
     * Default buffer length
     */
    static const unsigned int DEFAULT_LENGTH = 256;
protected:
    /**
     * Buffer
     */
	T *buffer;
    /**
     * Pointer to point the newest one's index
     */
    unsigned int newest;
    /**
     * Buffer length
     */
    const unsigned int LENGTH;
public:
    /**
     * Constructor.
     *
     * @param length Buffer length.
     * @param defaultValue Default value of contents of the buffer.
     * @return -
     */
    NewRingBuffer(
        unsigned long length = DEFAULT_LENGTH, 
        T defaultValue = 0)
        :
        LENGTH(length)
    {
        this->buffer = new T[this->LENGTH];
        for (unsigned int i = 0; i < this->LENGTH; i++) {
            buffer[i] = defaultValue;
        }
        this->initializeNewest();
    }

    /**
     * Destructor.
     *
     * @param -
     * @return -
     */
	~NewRingBuffer(void)
    {
        delete[] this->buffer;
    }
protected:
    /////////////////////////////////////////////////////////////////
    // <How Ring Buffer Works Internally>
    // push() puts an element from the tail to the head internally.
    // Then, the newest pointer moves from the tail to the head together. 
    // getIndex(i) forwards the index from the newest pointer to back.
    // | 0 | 0 | 0 |
    // push(1) => | 0 | 0 | 1 | and newest index == 2
    // push(2) => | 0 | 2 | 1 | and newest index == 1
    // push(3) => | 3 | 2 | 1 | and newest index == 0
    // push(4) => | 3 | 2 | 4 | and newest index == 2
    /////////////////////////////////////////////////////////////////

    /**
     * Convert the ring buffer index to the physical memory index.
     *
     * @param i Ring buffer index (0 denotes the newest value)
     * @return Physical memory index (index of buffer[] array)
     */
    inline unsigned int getIndex(unsigned int i) const
    {
         return (i + this->newest) % this->LENGTH;
    };
    /**
     * Initialize the newest  pointer. 
     *
     * @param -
     * @return -
     */
    inline void initializeNewest()
    {
         this->newest = 0;
    }
    /**
     * Forward the newest pointer.
     *
     * @param -
     * @return -
     */
    inline void forwardNewest()
    {
         this->newest = (this->newest == 0 ? this->LENGTH - 1 : this->newest - 1);
    }
public:
    /**
     * Get the length of the buffer
     *
     * @param -
     * @return length
     */
    inline unsigned int getLength() const
    {
        return this->LENGTH;
    }
    /**
     * Get the value at index i 
     * where 0 denotes the newest one and 
     * obj->getLength()-1 denotes the oldest one. 
     *
     * @param i index
     * @return value
     */
    inline T get(unsigned int i = 0) const
    {
        return this->buffer[this->getIndex(i)];
    }
    /**
     * Push the new value and remove the oldest value.
     *
     * @param newValue New value
     * @return Removed oldest value
     */
    T push(T newValue)
    {
        this->forwardNewest();
        T oldestValue = this->buffer[this->newest];
        this->buffer[this->newest] = newValue;
        return oldestValue;
    }
};

#endif // RINGBUFFER_H_
