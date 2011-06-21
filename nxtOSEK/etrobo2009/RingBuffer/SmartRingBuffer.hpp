#ifndef SMARTRINGBUFFER_H_
#define SMARTRINGBUFFER_H_

#include "boost/scoped_array.hpp"

/**
 * Ring Buffer Data Structure.
 */
template <class T>
class SmartRingBuffer
{
public:
    /**
     * Default buffer length
     */
    static const unsigned int DEFAULT_LENGTH = 256;
protected:
    /**
     * Smart pointer for Buffer
     */
    boost::scoped_array<T> buffer_ptr;
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
    SmartRingBuffer(
        unsigned long length = DEFAULT_LENGTH, 
        T defaultValue = 0)
        :
        LENGTH(length)
    {
        this->buffer_ptr.reset(new T[this->LENGTH]);
        T *buffer = this->buffer_ptr.get();
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
	~SmartRingBuffer(void)
    {
    }
public:
    /**
     * Get a direct memory access for a buffered array
     *
     * @return pointer for an array
     */
    inline T* getArray(void) const
    {
        return this->buffer_ptr.get();
    }
protected:
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
        T* buffer = this->buffer_ptr.get();
        return buffer[this->getIndex(i)];
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
        T *buffer = this->buffer_ptr.get();
        T oldestValue = buffer[this->newest];
        buffer[this->newest] = newValue;
        return oldestValue;
    }
};

#endif // SMARTRINGBUFFER_H_
