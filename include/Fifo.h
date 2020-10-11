/*
 * Fifo.h
 *
 *  Created on: Mar 20, 2019
 *      Author: viktor
 */

#ifndef FIFO_H_
#define FIFO_H_

#include <Arduino.h>
#include <stdexcept>

#ifndef CIRCULAR_FIFO_BUFFER_SIZE
#define CIRCULAR_FIFO_BUFFER_SIZE 100
#endif

template <class T>
class Fifo
{
private:
	int size = CIRCULAR_FIFO_BUFFER_SIZE;
	T storage[CIRCULAR_FIFO_BUFFER_SIZE];
	long begin = 0; // First occupied position
	long end = 0;	// First free position, new item will be placed tere

public:
	Fifo()
	{
		begin = 0;
		end = 0;
	}

	bool put(T item)
	{
		// Serial.println("Put before: begin=" + String(begin) + ", end=" + String(end));

		long newEnd = end + 1;
		if (newEnd >= size)
		{
			newEnd = 0;
		}
		if (newEnd == begin)
		{
			// No space left
			// Serial.println("Put exception: no space left in buffer. New end=" + String(newEnd) + ", begin=" + String(begin));
			return (false);
		}
		storage[end] = item;

		// Serial.println("Put: item stored at position " + String(end));

		end = newEnd;

		// Serial.println("Put after: begin=" + String(begin) + ", end=" + String(end));

		return (true);
	}
	T get(void)
	{
		// Serial.println("Get before: begin=" + String(begin) + ", end=" + String(end));
		if (begin == end)
		{
			// Buffer is empty
			throw std::underflow_error("Nothing to fetch because buffer is empty");
		}

		// Serial.println("Get: item retrieved at position " + String(begin));

		T result = storage[begin];
		begin++;
		if (begin >= size)
		{
			begin = 0;
		}

		// Serial.println("Get after: begin=" + String(begin) + ", end=" + String(end));

		return (result);
	}

	long length(void)
	{
		if (begin <= end)
		{
			return (end - begin);
		}

		return (size - begin + end);
	}

	void clear(void)
	{
		begin = end = 0;
	}
};

#endif /* FIFO_H_ */
