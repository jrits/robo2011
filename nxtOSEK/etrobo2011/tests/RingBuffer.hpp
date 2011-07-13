#ifndef RING_BUFFER_H_
#define RING_BUFFER_H_

/**
 * Ring Buffer Data Structure.
 *
 * @date Created by Naotoshi Seo on 10/07/27.
 * @copyright 2010 Naotoshi Seo. All rights reserved. 
 */
template <class T>
class RingBuffer
{
protected:
    T *buffer; //!< Buffer
    unsigned int head; //!< The head of the ring buffer
    const unsigned int LENGTH; //!< The buffer length
public:
    /**
     * Constructor.
     *
     * @param[in/out] buf The pointer of an array. The contents must've been initialized yet.
     * @param[in] length The buffer length.
     * @return -
     */
    RingBuffer(T *buf, unsigned int length)
        :
        buffer(buf),
        LENGTH(length)
    {
        //this->buffer = new T[this->LENGTH];
        //for (unsigned int i = 0; i < this->LENGTH; i++) {
        //    buffer[i] = defaultValue;
        //}
        this->head = 0;
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
     * Push the new value and remove the oldest value.
     *
     * @param newValue New value
     * @return Removed oldest value
     */
    T push(T newValue)
    {
        this->head = (this->head + 1) % this->LENGTH;
        T oldestValue = this->buffer[this->head];
        this->buffer[this->head] = newValue;
        return oldestValue;
    }
    /**
     * Get a value at an index.
     *
     * @param[in] i Ring buffer index.
     *   0 denotes the head value, and negatives denote the old values.
     *   You may use +1 to denote the oldest value. 
     * @return value
     */
    T get(int i = 0) const
    {
        unsigned int index = (i + this->head + this->LENGTH) % this->LENGTH;
        return this->buffer[index];
    }
};

#endif // RING_BUFFER_H_
