#ifndef RING_BUFFER_H_
#define RING_BUFFER_H_

/**
 * Ring Buffer Data Structure.
 */
template <class T>
class RingBuffer
{
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
    RingBuffer(
        T *buf,
        unsigned int length,
        T defaultValue = 0)
        :
        buffer(buf),
        LENGTH(length)
    {
        //this->buffer = new T[this->LENGTH];
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
	~RingBuffer(void)
    {
        //delete[] this->buffer;
    }
protected:
    /**
     * Convert the ring buffer index to the physical memory index.
     *
     * @param i Ring buffer index.
     *   0 denotes the newest value, and negatives denote the old values.
     *   You may use +1 to denote the oldest value. 
     * @return Physical memory index (index of buffer[] array)
     */
    inline unsigned int getIndex(int i) const
    {
         return (i + this->newest + this->LENGTH) % this->LENGTH;
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
        this->newest = (this->newest + 1) % this->LENGTH;
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
     * @param i Ring buffer index.
     *   0 denotes the newest value, and negatives denote the old values.
     *   You may use +1 to denote the oldest value. 
     *
     * @param i index
     * @return value
     */
    inline T get(int i = 0) const
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

#endif // RING_BUFFER_H_
