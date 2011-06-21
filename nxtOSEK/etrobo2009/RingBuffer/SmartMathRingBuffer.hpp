#ifndef SMARTMATHRINGBUFFER_H_
#define SMARTMATHRINGBUFFER_H_

#include "boost/scoped_array.hpp"
#include "SmartRingBuffer.hpp"

/**
 * Ring Buffer Data Structure extending statistical functions
 */
template <class T>
class SmartMathSmartRingBuffer : public SmartRingBuffer<T>
{
protected:
    /**
     * Sum of elements.
     */
    double sum;
    /**
     * Average of elements.
     */
    double avg;
    /**
     * Difference between the newest and the oldest element.
     */
    double diff;
public:
    /**
     * Constructor.
     *
     * @param length Buffer length.
     * @param defaultValue Default value of contents of the buffer.
     * @return -
     */
    SmartMathSmartRingBuffer(
        unsigned long length = SmartRingBuffer<T>::DEFAULT_LENGTH, 
        T defaultValue = 0)
        :
        SmartRingBuffer<T>(length, defaultValue)
    {
        this->sum = defaultValue * length;
        this->avg = defaultValue;
    }

    /**
     * Destructor.
     *
     * @param -
     * @return -
     */
	~SmartMathSmartRingBuffer(void)
    {
    }
public:
    /**
     * Get the sum of elements.
     * 
     * @return sum of elements
     */
    inline double getSum(void)
    {
        return this->sum;
    }
    /**
     * Get the average of elements.
     * 
     * @return average of elements
     */
    inline double getAvg(void)
    {
        return this->avg;
    }
    /**
     * Get the difference between the newest and the oldest element.
     * 
     * @return difference between the newest and the oldest
     */
    inline double getDiff(void)
    {
        return this->diff;
    }
public:
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

        this->diff = newValue - oldestValue;
        this->sum += diff;
        this->avg += diff / (double)this->LENGTH;

        return oldestValue;
    }
};

#endif // SMARTMATHRINGBUFFER_H_
