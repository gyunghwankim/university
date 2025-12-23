#ifndef GRAYSCALE_H
#define GRAYSCALE_H



#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

#define _USE_MATH_DEFINES  // ��� ����� ���ؼ� math.h���� ������ �����Ѵ�.
#include <math.h>

#include "../../allocTable.h" // ���� ��η� ���� ���ؼ� "../" ����Ѵ�.


#ifndef MAX
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#endif // MAX

#ifndef MIN
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#endif // MIN

#ifndef CLAMP
#define CLAMP(x, minValue, maxValue) ((x) < (minValue) ? (minValue) : ((x) > (maxValue) ? (maxValue) : (x)))
#endif // CLAMP

#ifndef ABS
#define ABS(x) ((x) < 0 ? -(x) : (x))
#endif // ABS

#ifndef SWAP
#define SWAP(x, y, TEMP) ((TEMP) = (x), (x) = (y), (y) = (TEMP))
#endif // SWAP

#ifndef ROUND
#define ROUND(x) ((x)>=0?(long)((x)+0.5):(long)((x)-0.5))
#endif // ROUND

#define _8bit 256
#define BORDER_CONSTANT 0		// padding �߰� ������ ��� 0���� ä���. (zero-padding)
#define BORDER_REPLICATE 1		// �����ڸ� ȭ���� ������ ä���, �������� ȭ�Ҵ� �α� ȭ���� ������� ä���.
#define BORDER_REFLECT 2		// �������� ����.
#define BORDER_REFLECT_101 3	// �������� ����.
#define _3by3 3
#define _5by5 5
#define _7by7 7
#define _9by9 9

enum PaddingMode {
	paddingModeValid,
	paddingModeSame,
	// paddingModeFull
};

enum PaddingMethod {
	paddingMethodConstant,
	paddingMethodReplicate // == edge padding
	// others not implement
};





typedef struct tag_grayscale
{
	uint8_t* pBuffer;
	size_t width;
	size_t height;
}Grayscale;



inline void selectionSort(uint8_t _arr[], int32_t _count);

//�ѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤ�
//�� code simplification
//�ѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤ�

Grayscale myFread(const char* _FileName, size_t _width, size_t _height)
{
	FILE* fp;
	if (fopen_s(&fp, _FileName, "rb"))
		assert(NULL);
	Grayscale output = { NULL, _width, _height };
	output.pBuffer = (unsigned char*)myMalloc(sizeof(unsigned char) * output.width * output.height);
	if (!(fread_s(output.pBuffer, sizeof(unsigned char) * output.width * output.height, sizeof(unsigned char), output.width * output.height, fp)))
		assert(NULL);

	fclose(fp);
	return output;
}
void createRawImage(const char* _FileName, const Grayscale _Src)
{
	FILE* fp;
	if (fopen_s(&fp, _FileName, "wb"))
		assert(NULL);
	if (!(fwrite(_Src.pBuffer, sizeof(unsigned char), _Src.width * _Src.height, fp)))
		assert(NULL);
	fclose(fp);
}





//�ѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤ�
//�� spatial filtering
//�ѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤ�

/*
	normalizeArray

	������ _buffer�� �����ϸ�, �簡�� �� �����Ͱ� ���� �� �ִ�.
*/
void* normalizeArray(void* _buffer, const size_t _Count, const int _minimum, const int _maximum, const char* _outputDataType)
{
	float MIN, MAX;
	int i;
	if ("unsigned char" == _outputDataType && 0 <= _minimum && 255 >= _maximum)
	{
		MIN = MAX = (float)(((unsigned char*)_buffer)[0]);
		for (i = 0; i < _Count; ++i)
		{
			MAX = ((unsigned char*)_buffer)[i] > MAX ? ((unsigned char*)_buffer)[i] : MAX;
			MIN = ((unsigned char*)_buffer)[i] < MIN ? ((unsigned char*)_buffer)[i] : MIN;
		}

		unsigned char* output = (unsigned char*)myMalloc(sizeof(unsigned char), _Count);

		for (i = 0; i < _Count; ++i)
		{
			output[i] = (unsigned char)(((float)(((unsigned char*)_buffer)[i]) - MIN) * (float)(_maximum - _minimum) / (MAX - MIN) + (float)_minimum);
		}

		subAlloc(_buffer);
		return output;
	}
	else if ("int" == _outputDataType)
	{
		MIN = MAX = (float)(((int*)_buffer)[0]);
		for (i = 0; i < _Count; ++i)
		{
			MAX = ((int*)_buffer)[i] > MAX ? ((int*)_buffer)[i] : MAX;
			MIN = ((int*)_buffer)[i] < MIN ? ((int*)_buffer)[i] : MIN;
		}

		int* output = (int*)myMalloc(sizeof(int), _Count);

		for (i = 0; i < _Count; ++i)
		{
			output[i] = (int)(((float)(((int*)_buffer)[i]) - MIN) * (float)(_maximum - _minimum) / (MAX - MIN) + (float)_minimum);
		}

		subAlloc(_buffer);
		return output;
	}
	else if ("float" == _outputDataType)
	{
		MIN = MAX = (float)(((float*)_buffer)[0]);
		for (i = 0; i < _Count; ++i)
		{
			MAX = ((float*)_buffer)[i] > MAX ? ((float*)_buffer)[i] : MAX;
			MIN = ((float*)_buffer)[i] < MIN ? ((float*)_buffer)[i] : MIN;
		}

		float* output = (float*)myMalloc(sizeof(float), _Count);

		for (i = 0; i < _Count; ++i)
		{
			output[i] = (((float)(((float*)_buffer)[i]) - MIN) * (float)(_maximum - _minimum) / (MAX - MIN) + (float)_minimum);
		}

		subAlloc(_buffer);
		return output;
	}
	else
	{
		assert(NULL);
		return NULL;
	}
}





uint32_t* normalizeArray(const uint32_t* _buffer, size_t _elementCount, uint32_t _min, uint32_t _max)
{
	// _min < _max
	float targetMin = (float)MIN(_min, _max);
	float targetMax = (float)MAX(_min, _max);


	// _buffer�� min, max�� ã�´�.
	float sourceMin, sourceMax;
	sourceMin = sourceMax = (float)_buffer[0];

	for (size_t i = 0; i < _elementCount; ++i)
	{
		sourceMin = MIN((float)_buffer[i], sourceMin);
		sourceMax = MAX((float)_buffer[i], sourceMax);
	}

	
	uint32_t* normalizedBuffer = (uint32_t*)myMalloc(sizeof(uint32_t) * _elementCount);


	// ����ȭ �ڵ忡�� ratio�� �и� 0�� �� ���� ó��
	if (sourceMax == sourceMin)
	{
		memcpy(normalizedBuffer, _buffer, sizeof(uint32_t) * _elementCount);
	}
	// �ݿø� ���� ����ȭ
	else
	{
		float ratio = (targetMax - targetMin) / (sourceMax - sourceMin);
		for (size_t i = 0; i < _elementCount; ++i)
		{
			normalizedBuffer[i] = (uint32_t)roundf(((float)_buffer[i] - sourceMin) * ratio + targetMin);
		}
	}


	return normalizedBuffer;
}





/*
	normalizeArray_embossing

	������ _buffer�� �����ϸ�, �簡�� �� �����Ͱ� ���� �� �ִ�.
	HPF�� ��ġ�� embossing �����̸�, ��� ������ ������ �츮�� ���Ⱚ�� 127or128�̴�.
*/
void* normalizeArray_embossing(void* _buffer, const size_t _Count, const int _minimum, const int _maximum, const char* _outputDataType)
{
	if (_minimum <= _maximum)
		assert(NULL);
	float MIN, MAX;
	int i;
	if ("unsigned char" == _outputDataType && 0 <= _minimum && 255 >= _maximum)
	{
		MIN = MAX = (float)(((unsigned char*)_buffer)[0]);
		for (i = 0; i < _Count; ++i)
		{
			MAX = ((unsigned char*)_buffer)[i] > MAX ? ((unsigned char*)_buffer)[i] : MAX;
			MIN = ((unsigned char*)_buffer)[i] < MIN ? ((unsigned char*)_buffer)[i] : MIN;
		}

		if (0 <= MAX)
		{
			if (0 <= MIN)
			{
				MIN = -MAX;
			}
			else
			{
				if (MAX >= -MIN)
				{
					MIN = -MAX;
				}
				else
				{
					MAX = -MIN;
				}
			}
		}
		else
		{
			MAX = -MIN;
		}

		unsigned char* output = (unsigned char*)myMalloc(sizeof(unsigned char) * _Count);

		for (i = 0; i < _Count; ++i)
		{
			output[i] = (unsigned char)(((float)(((unsigned char*)_buffer)[i]) - MIN) * (float)(_maximum - _minimum) / (MAX - MIN) + (float)_minimum);
		}

		subAlloc(_buffer);
		return output;
	}
	else if ("int" == _outputDataType)
	{
		MIN = MAX = (float)(((int*)_buffer)[0]);
		for (i = 0; i < _Count; ++i)
		{
			MAX = ((int*)_buffer)[i] > MAX ? ((int*)_buffer)[i] : MAX;
			MIN = ((int*)_buffer)[i] < MIN ? ((int*)_buffer)[i] : MIN;
		}

		if (0 <= MAX)
		{
			if (0 <= MIN)
			{
				MIN = -MAX;
			}
			else
			{
				if (MAX >= -MIN)
				{
					MIN = -MAX;
				}
				else
				{
					MAX = -MIN;
				}
			}
		}
		else
		{
			MAX = -MIN;
		}

		int* output = (int*)myMalloc(sizeof(int) * _Count);

		for (i = 0; i < _Count; ++i)
		{
			output[i] = (int)(((float)(((int*)_buffer)[i]) - MIN) * (float)(_maximum - _minimum) / (MAX - MIN) + (float)_minimum);
		}

		subAlloc(_buffer);
		return output;
	}
	else if ("float" == _outputDataType)
	{
		MIN = MAX = (float)(((float*)_buffer)[0]);
		for (i = 0; i < _Count; ++i)
		{
			MAX = ((float*)_buffer)[i] > MAX ? ((float*)_buffer)[i] : MAX;
			MIN = ((float*)_buffer)[i] < MIN ? ((float*)_buffer)[i] : MIN;
		}

		if (0 <= MAX)
		{
			if (0 <= MIN)
			{
				MIN = -MAX;
			}
			else
			{
				if (MAX >= -MIN)
				{
					MIN = -MAX;
				}
				else
				{
					MAX = -MIN;
				}
			}
		}
		else
		{
			MAX = -MIN;
		}

		float* output = (float*)myMalloc(sizeof(float) * _Count);

		for (i = 0; i < _Count; ++i)
		{
			output[i] = (((float)(((float*)_buffer)[i]) - MIN) * (float)(_maximum - _minimum) / (MAX - MIN) + (float)_minimum);
		}

		subAlloc(_buffer);
		return output;
	}
	else
	{
		assert(NULL);
		return NULL;
	}
}




/*
	add_SaltAndPepperNoise
*/
Grayscale add_SaltAndPepperNoise(const Grayscale _Src, int _noiseCount)
{
	// create new image, and copy _Src to new.
	Grayscale output = { NULL, _Src.width, _Src.height };
	output.pBuffer = (unsigned char*)myMalloc(sizeof(unsigned char) * output.width * output.height);

	int i, copyLoop = (int)(output.width * output.height);
	for (int i = 0; i < copyLoop; ++i)
		output.pBuffer[i] = _Src.pBuffer[i];



	// ������ ������ ��ġ�� 0 �Ǵ� 255�� ���� ������ ���� _noiseCount�� ��´�.
	// �� ���� ���� ��ġ�� �ʴ� �������� ����ϸ�, ���� ������ ������ ����.
	srand((unsigned int)time(NULL));
	const int imageSize = (int)(output.width * output.height);
	int blackwhite;
	while (_noiseCount)
	{
		i = rand() % imageSize;

		blackwhite = (_noiseCount % 2) * 255;

		if (blackwhite != output.pBuffer[i])
		{
			output.pBuffer[i] = blackwhite;

			--_noiseCount;
		}
	}



	return output;
}





/*
	padding
	[source], [maskSize: 3(3by3), 5(5by5), ...], [padding mode: valid, same, full], [padding value: constant, replicate], [constant value: 0 ~ 255]

	filter���� ����ϰ� �� �����ϼ���. (valid padding�� source�� �����Ͽ� ���ο� ��ü�� �����ϴ�.)
*/
Grayscale padding(const Grayscale _src, int32_t _maskSize, enum PaddingMode _paddingMode, enum PaddingMethod _paddingMethod, uint8_t _valueForConstantPadding)
{
	Grayscale dst = { NULL, };

	size_t paddingSize = _maskSize / 2;
	size_t y, x, srcIndex, dstIndex;
	switch (_paddingMode)
	{
	case paddingModeValid:
		dst.width = _src.width;
		dst.height = _src.height;
		size_t bufferSize = sizeof(uint8_t) * dst.width * dst.height;
		dst.pBuffer = (uint8_t*)myMalloc(bufferSize);

		// copy the same
		memcpy(dst.pBuffer, _src.pBuffer, bufferSize);
		break;

	case paddingModeSame:
		switch (_paddingMethod)
		{
		case paddingMethodConstant:
			dst.width = _src.width + paddingSize * 2;
			dst.height = _src.height + paddingSize * 2;
			dst.pBuffer = (uint8_t*)myMalloc(sizeof(uint8_t) * dst.width * dst.height);

			// constant padding: set to a specific value
			memset(dst.pBuffer, _valueForConstantPadding, sizeof(uint8_t) * dst.width * dst.height);

			// memcpy: copy from source to dest(center of image)
			for (y = 0; y < _src.height; ++y)
			{
				for (x = 0; x < _src.width; ++x)
				{
					srcIndex = (y)*_src.width + (x);
					dstIndex = (y + paddingSize) * dst.width + (x + paddingSize);
					dst.pBuffer[dstIndex] = _src.pBuffer[srcIndex];
				}
			}
			break;

		case paddingMethodReplicate:
			dst.width = _src.width + paddingSize * 2;
			dst.height = _src.height + paddingSize * 2;
			dst.pBuffer = (uint8_t*)myMalloc(sizeof(uint8_t) * dst.width * dst.height);

			// memcpy: copy from source to dest(center of image)
			for (y = 0; y < _src.height; ++y)
			{
				for (x = 0; x < _src.width; ++x)
				{
					srcIndex = (y)*_src.width + (x);
					dstIndex = (y + paddingSize) * dst.width + (x + paddingSize);
					dst.pBuffer[dstIndex] = _src.pBuffer[srcIndex];
				}
			}

			// replicate: �ڳʸ� ������ ó������ �ʰ�, �����ڸ� �κ��� ����-�¿� ������ �ִ� ������ �����ϸ� �ڳ� �κе� �� �ȼ� ������ ����Ǵ� ����� ����Ѵ�.
			// ��
			for (y = 0; y < paddingSize; ++y)
				for (x = 0; x < dst.width; ++x)
					dst.pBuffer[y * dst.width + x] = dst.pBuffer[paddingSize * dst.width + x];
			// ��
			for (y = dst.height - paddingSize; y < dst.height; ++y)
				for (x = 0; x < dst.width; ++x)
					dst.pBuffer[y * dst.width + x] = dst.pBuffer[(dst.height - 1 - paddingSize) * dst.width + x];
			// ��
			for (y = 0; y < dst.height; ++y)
				for (x = 0; x < paddingSize; ++x)
					dst.pBuffer[y * dst.width + x] = dst.pBuffer[y * dst.width + paddingSize];
			// ��
			for (y = 0; y < dst.height; ++y)
				for (x = dst.width - paddingSize; x < dst.width; ++x)
					dst.pBuffer[y * dst.width + x] = dst.pBuffer[y * dst.width + (dst.width - 1 - paddingSize)];
			break;

		default:
			// exception
			break;
		}
		break;

	default:
		// exception
		break;
	}

	return dst;
}




/*
	linear filter
	[source], [maskSize: 3(3by3), 5(5by5), ...], [padding mode: valid, same, full], [padding value: constant, replicate], [constant value: 0 ~ 255]
*/
Grayscale linearFilter(const Grayscale _src, const double _mask[], int32_t _maskSize, enum PaddingMode _paddingMode, enum PaddingMethod _paddingMethod, uint8_t _valueForConstantPadding)
{
	// _src���, _src�� padding�� src�� ����Ѵ�. _paddingMode�� ���� ���� src���� ũ�Ⱑ Ŭ ���� �ִ�.
	Grayscale src = padding(_src, _maskSize, _paddingMode, _paddingMethod, _valueForConstantPadding);

	Grayscale dst = { NULL, };
	int32_t paddingSize = _maskSize / 2;
	dst.width = src.width - (size_t)(2 * paddingSize);
	dst.height = src.height - (size_t)(2 * paddingSize);
	dst.pBuffer = (uint8_t*)myMalloc(sizeof(uint8_t) * dst.width * dst.height);

	for (int32_t y = paddingSize; y < (int32_t)src.height - paddingSize; ++y)
	{
		for (int32_t x = paddingSize; x < (int32_t)src.width - paddingSize; ++x)
		{
			double weightedSum = 0;

			for (int32_t j = -paddingSize; j <= paddingSize; ++j)
			{
				for (int32_t i = -paddingSize; i <= paddingSize; ++i)
				{
					int32_t srcIndex = (y + j) * (int32_t)src.width + (x + i);
					int32_t maskindex = (j + paddingSize) * _maskSize + (i + paddingSize);
					weightedSum += (double)src.pBuffer[srcIndex] * _mask[maskindex];
				}
			}

			int32_t dstIndex = (y - paddingSize) * (int32_t)dst.width + (x - paddingSize);
			dst.pBuffer[dstIndex] = (uint8_t)CLAMP(ROUND(weightedSum), 0, 255);
		}
	}
	myFree(src.pBuffer);
	return dst;
}





/*
	mean filter
*/
Grayscale mean_filter(const Grayscale _Src, const int _maskConst, const int _paddingConst)
{
	// buffer_padding ����.
	float* padding = Padding(_Src, _maskConst, _paddingConst);



	// ��� ���� ����.
	Grayscale output = { NULL, _Src.width, _Src.height };
	output.pBuffer = (unsigned char*)myMalloc(sizeof(unsigned char) * output.width * output.height);



	// ���� ���Ǽ��� ���� buffer_padding�� width/height ����.
	const int L = (_maskConst - 1) / 2;
	const int width_padding = (int)_Src.width + 2 * L;
	const int height_padding = (int)_Src.height + 2 * L;



	// mean filter
	// kernel�� ����� ��� 1�̹Ƿ�, mask�� ��ġ�� �ʴ´�.
	const int xLoop = width_padding - L;
	const int yLoop = height_padding - L;
	const float maskSize = (float)(_maskConst * _maskConst);
	float SUM;
	int x, y, i, j;
	for (y = L; y < yLoop; ++y)
	{
		for (x = L; x < xLoop; ++x)
		{
			// mean mask
			SUM = 0;
			for (j = -L; j <= +L; ++j)
			{
				for (i = -L; i <= +L; ++i)
				{
					SUM += padding[width_padding * (y + j) + (x + i)];
				}
			}

			output.pBuffer[(int)output.width * (y - L) + (x - L)] = (unsigned char)(SUM / maskSize);
		}
	}



	// buffer_padding�� �����ϰ�, ����� ��ȯ�Ѵ�.
	subAlloc(padding);

	return output;
}





//void gaussian_filter(const Grayscale _Src, const int _maskConst, const int _paddingConst) { /*�̱���*/ }





//void bilateral_filter(const Grayscale _Src, const int _maskConst, const int _paddingConst) { /*�̱���*/ }




/*
	min filter
*/
Grayscale min_filter(const Grayscale _Src, const int _maskConst, const int _paddingConst)
{
	// buffer_padding ����.
	float* padding = Padding(_Src, _maskConst, _paddingConst);



	// ��� ���� ����.
	Grayscale output = { NULL, _Src.width, _Src.height };
	output.pBuffer = (unsigned char*)myMalloc(sizeof(unsigned char) * output.width * output.height);



	// ���� ���Ǽ��� ���� buffer_padding�� width/height ����.
	const int L = (_maskConst - 1) / 2;
	const int width_padding = (int)_Src.width + 2 * L;
	const int height_padding = (int)_Src.height + 2 * L;



	// min filter
	// ���İ� mask�� ��ġ�� �ʰ� ���׿����ڸ� ����Ͽ� �ٷ� �ּڰ��� �����Ѵ�.
	const int xLoop = width_padding - L;
	const int yLoop = height_padding - L;
	float MIN;
	int x, y, i, j;
	for (y = L; y < yLoop; ++y)
	{
		for (x = L; x < xLoop; ++x)
		{
			// min mask
			MIN = 255;
			for (j = -L; j <= +L; ++j)
			{
				for (i = -L; i <= +L; ++i)
				{
					MIN = MIN < padding[width_padding * (y + j) + (x + i)] ? MIN : padding[width_padding * (y + j) + (x + i)];
				}
			}

			output.pBuffer[(int)output.width * (y - L) + (x - L)] = (unsigned char)MIN;
		}
	}



	// buffer_padding�� �����ϰ�, ����� ��ȯ�Ѵ�.
	subAlloc(padding);

	return output;
}




/*
	max filter
*/
Grayscale max_filter(const Grayscale _Src, const int _maskConst, const int _paddingConst)
{
	// buffer_padding ����.
	float* padding = Padding(_Src, _maskConst, _paddingConst);



	// ��� ���� ����.
	Grayscale output = { NULL, _Src.width, _Src.height };
	output.pBuffer = (unsigned char*)myMalloc(sizeof(unsigned char) * output.width * output.height);



	// ���� ���Ǽ��� ���� buffer_padding�� width/height ����.
	const int L = (_maskConst - 1) / 2;
	const int width_padding = (int)_Src.width + 2 * L;
	const int height_padding = (int)_Src.height + 2 * L;



	// max filter
	// ���İ� mask�� ��ġ�� �ʰ� ���׿����ڸ� ����Ͽ� �ٷ� �ִ��� �����Ѵ�.
	const int xLoop = width_padding - L;
	const int yLoop = height_padding - L;
	float MAX;
	int x, y, i, j;
	for (y = L; y < yLoop; ++y)
	{
		for (x = L; x < xLoop; ++x)
		{
			// max mask
			MAX = 0;
			for (j = -L; j <= +L; ++j)
			{
				for (i = -L; i <= +L; ++i)
				{
					MAX = MAX > padding[width_padding * (y + j) + (x + i)] ? MAX : padding[width_padding * (y + j) + (x + i)];
				}
			}

			output.pBuffer[(int)output.width * (y - L) + (x - L)] = (unsigned char)MAX;
		}
	}



	// buffer_padding�� �����ϰ�, ����� ��ȯ�Ѵ�.
	subAlloc(padding);

	return output;
}




/*
	midpoint filter
*/
Grayscale midPoint_filter(const Grayscale _Src, const int _maskConst, const int _paddingConst)
{
	// buffer_padding ����.
	float* padding = Padding(_Src, _maskConst, _paddingConst);



	// ��� ���� ����.
	Grayscale output = { NULL, _Src.width, _Src.height };
	output.pBuffer = (unsigned char*)myMalloc(sizeof(unsigned char) * output.width * output.height);



	// ���� ���Ǽ��� ���� buffer_padding�� width/height ����.
	const int L = (_maskConst - 1) / 2;
	const int width_padding = (int)_Src.width + 2 * L;
	const int height_padding = (int)_Src.height + 2 * L;



	// mid-point filter
	// ���İ� mask�� ��ġ�� �ʰ� ���׿����ڸ� ����Ͽ� �ٷ� �ּڰ��� �ִ��� ���ϰ�, �� ���� ����� �����Ѵ�.
	const int xLoop = width_padding - L;
	const int yLoop = height_padding - L;
	float MIN, MAX;
	int x, y, i, j;
	for (y = L; y < yLoop; ++y)
	{
		for (x = L; x < xLoop; ++x)
		{
			// mid-point mask
			MIN = 255, MAX = 0;
			for (j = -L; j <= +L; ++j)
			{
				for (i = -L; i <= +L; ++i)
				{
					MIN = MIN < padding[width_padding * (y + j) + (x + i)] ? MIN : padding[width_padding * (y + j) + (x + i)];
					MAX = MAX > padding[width_padding * (y + j) + (x + i)] ? MAX : padding[width_padding * (y + j) + (x + i)];
				}
			}

			output.pBuffer[(int)output.width * (y - L) + (x - L)] = (unsigned char)((MIN + MAX) / 2);
		}
	}



	// buffer_padding�� �����ϰ�, ����� ��ȯ�Ѵ�.
	subAlloc(padding);

	return output;
}





/*
	median filter
*/
Grayscale medianFilter(const Grayscale _src, int32_t _maskSize, enum PaddingMode _paddingMode, enum PaddingMethod _paddingMethod, uint8_t _valueForConstantPadding)
{
	Grayscale src = padding(_src, _maskSize, _paddingMode, _paddingMethod, _valueForConstantPadding);

	Grayscale dst = { NULL, };
	int32_t paddingSize = _maskSize / 2;
	dst.width = src.width - (size_t)(2 * paddingSize);
	dst.height = src.height - (size_t)(2 * paddingSize);
	dst.pBuffer = (uint8_t*)myMalloc(sizeof(uint8_t) * dst.width * dst.height);

	int32_t maskArea = _maskSize * _maskSize;
	uint8_t* mask = (uint8_t*)myMalloc(sizeof(uint8_t) * maskArea);
	for (int32_t y = paddingSize; y < (int32_t)src.height - paddingSize; ++y)
	{
		for (int32_t x = paddingSize; x < (int32_t)src.width - paddingSize; ++x)
		{
			// _source�� mask�� ����
			int32_t maskIndex = 0;
			for (int32_t j = -paddingSize; j <= paddingSize; ++j)
			{
				for (int32_t i = -paddingSize; i <= paddingSize; ++i)
				{
					int32_t bufferIndex = (y + j) * (int32_t)src.width + (x + i);
					mask[maskIndex++] = src.pBuffer[bufferIndex];
				}
			}

			// mask ����
			selectionSort(mask, maskArea);

			// �߰��� ����
			uint8_t medianValue = mask[maskArea / 2];
			dst.pBuffer[(y - paddingSize) * (int32_t)dst.width + (x - paddingSize)] = medianValue;
		}
	}
	myFree(mask);
	myFree(src.pBuffer);
	return dst;
}





/*
	alpha-trimmed mean filter
*/
Grayscale alpha_trimmed_mean_filter(const Grayscale _Src, const int _maskConst, const int _paddingConst, const float _alpha)
{
	// buffer_padding ����.
	float* padding = Padding(_Src, _maskConst, _paddingConst);



	// ��� ���� ����.
	Grayscale output = { NULL, _Src.width, _Src.height };
	output.pBuffer = (unsigned char*)myMalloc(sizeof(unsigned char) * output.width * output.height);



	// ���� ���Ǽ��� ���� buffer_padding�� width/height ����.
	const int L = (_maskConst - 1) / 2;
	const int width_padding = (int)_Src.width + 2 * L;
	const int height_padding = (int)_Src.height + 2 * L;



	/*
		alpha-trimmed mean filter
		����-���� �� ���� mask�迭�� ��հ��� �����Ѵ�.
	*/
	// mask�迭 ����.
	float* mask = (float*)myMalloc(sizeof(float) * (size_t)_maskConst * _maskConst);
	int maskCount;
	const int maskSize = _maskConst * _maskConst;
	const int loopBegin = (int)(maskSize * _alpha);
	const int loopEnd = maskSize - loopBegin;
	const float remainder = (float)(maskSize - 2 * loopBegin);

	const int xLoop = width_padding - L;
	const int yLoop = height_padding - L;

	int x, y, i, j;
	float TEMP_SWAP, SUM;
	for (y = L; y < yLoop; ++y)
	{
		for (x = L; x < xLoop; ++x)
		{
			maskCount = 0;
			for (j = -L; j <= +L; ++j)
			{
				for (i = -L; i <= +L; ++i)
				{
					mask[maskCount] = padding[width_padding * (y + j) + (x + i)];

					++maskCount;
				}
			}

			// selection sort
			for (i = 0; i < maskSize - 1; ++i)
			{
				for (j = i + 1; j < maskSize; ++j)
				{
					if (mask[i] > mask[j])
						SWAP(mask[i], mask[j], TEMP_SWAP);
				}
			}

			// alpha-trimmed mean
			SUM = 0;
			for (i = loopBegin; i < loopEnd; ++i)
			{
				SUM += mask[i];
			}

			output.pBuffer[(int)output.width * (y - L) + (x - L)] = (unsigned char)(SUM / remainder);
		}
	}


	// buffer_padding�� buffer_mask�� �����ϰ�, ����� ��ȯ�Ѵ�.
	subAlloc(padding);
	subAlloc(mask);

	return output;
}




/*
	low-pass linear filter
*/
Grayscale LowPass_linear_filter(const Grayscale _Src, const int _maskConst, const int _paddingConst, const float* _kernel)
{
	// buffer_padding ����.
	float* padding = Padding(_Src, _maskConst, _paddingConst);



	// ��� ���� ����.
	Grayscale output = { NULL, _Src.width, _Src.height };
	output.pBuffer = (unsigned char*)myMalloc(sizeof(unsigned char) * output.width * output.height);



	// ���� ���Ǽ��� ���� buffer_padding�� width/height ����.
	const int L = (_maskConst - 1) / 2;
	const int width_padding = (int)_Src.width + 2 * L;
	const int height_padding = (int)_Src.height + 2 * L;



	// LowPass linear filter
	const int xLoop = width_padding - L;
	const int yLoop = height_padding - L;
	float weightedSUM;
	int x, y, i, j;
	for (y = L; y < yLoop; ++y)
	{
		for (x = L; x < xLoop; ++x)
		{
			// correlation
			weightedSUM = 0;
			for (j = -L; j <= +L; ++j)
			{
				for (i = -L; i <= +L; ++i)
				{
					weightedSUM += padding[width_padding * (y + j) + (x + i)] * _kernel[_maskConst * (j + L) + (i + L)];
				}
			}

			output.pBuffer[(int)output.width * (y - L) + (x - L)] = (unsigned char)weightedSUM;
		}
	}



	// buffer_padding�� �����ϰ�, ����� ��ȯ�Ѵ�.
	subAlloc(padding);

	return output;
}




/*
	high-pass linear filter
*/
Grayscale HighPass_linear_filter(const Grayscale _Src, const int _maskConst, const int _paddingConst, const float* _kernel)
{
	// buffer_padding ����.
	float* padding = Padding(_Src, _maskConst, _paddingConst);



	// ��� ���� ����.
	Grayscale output = { NULL, _Src.width, _Src.height };
	output.pBuffer = (unsigned char*)myMalloc(sizeof(unsigned char) * output.width * output.height);



	// ���� ���Ǽ��� ���� buffer_padding�� width/height ����.
	const int L = (_maskConst - 1) / 2;
	const int width_padding = (int)_Src.width + 2 * L;
	const int height_padding = (int)_Src.height + 2 * L;



	// HighPass linear filter
	const int xLoop = width_padding - L;
	const int yLoop = height_padding - L;
	int x, y, i, j;
	float weightedSUM;
	for (y = L; y < yLoop; ++y)
	{
		for (x = L; x < xLoop; ++x)
		{
			// correlation
			weightedSUM = 0;
			for (j = -L; j <= +L; ++j)
			{
				for (i = -L; i <= +L; ++i)
				{
					weightedSUM += padding[width_padding * (y + j) + (x + i)] * _kernel[_maskConst * (j + L) + (i + L)];
				}
			}

			// overflow
			if (0 > weightedSUM)
				weightedSUM = 0;
			if (255 < weightedSUM)
				weightedSUM = 255;

			output.pBuffer[(int)output.width * (y - L) + (x - L)] = (unsigned char)weightedSUM;
		}
	}



	// buffer_padding�� �����ϰ�, ����� ��ȯ�Ѵ�.
	subAlloc(padding);

	return output;
}




/*
	embossing filter
*/
Grayscale embossing_filter(const Grayscale _Src, const int _maskConst, const int _paddingConst, const float* _kernel)
{
	// buffer_padding ����. �߰��� ����ȭ�� ���� buffer_TEMP�� ����.
	float* padding = Padding(_Src, _maskConst, _paddingConst);
	float* TEMP = (float*)myMalloc(sizeof(float) * _Src.width * _Src.height);


	// ��� ���� ����.
	Grayscale output = { NULL, _Src.width, _Src.height };
	output.pBuffer = (unsigned char*)myMalloc(sizeof(unsigned char) * output.width * output.height);



	// ���� ���Ǽ��� ���� buffer_padding�� width/height ����.
	const int L = (_maskConst - 1) / 2;
	const int width_padding = (int)_Src.width + 2 * L;
	const int height_padding = (int)_Src.height + 2 * L;



	// embossing filter (���Ⱚ 128)
	// �����ĸ� �����Ѵ�.  ->  [-128, 127] ������ ����ȭ�Ѵ�.  ->  ���Ⱚ���� + 128�� ���Ѵ�.
	// �ᱹ�� ���� ���� ��°����İ� 255�� ���� ������, ���� ���� �ϰ������İ� 0�� ���� ������ �ȴ�.
	// ������ ����.
	const int xLoop = width_padding - L;
	const int yLoop = height_padding - L;
	int x, y, i, j;
	float weightedSUM;
	for (y = L; y < yLoop; ++y)
	{
		for (x = L; x < xLoop; ++x)
		{
			// correlation
			weightedSUM = 0;
			for (j = -L; j <= +L; ++j)
			{
				for (i = -L; i <= +L; ++i)
				{
					weightedSUM += padding[width_padding * (y + j) + (x + i)] * _kernel[_maskConst * (j + L) + (i + L)];
				}
			}

			TEMP[(int)output.width * (y - L) + (x - L)] = weightedSUM;
		}
	}



	// [-128, +127] ������ ����ȭ.
	TEMP = (float*)normalizeArray_embossing(TEMP, output.width * output.height, 0, 255, "float");


	int iLoop = (int)(output.width * output.height);
	for (i = 0; i < iLoop; ++i)
	{
		output.pBuffer[i] = (unsigned char)TEMP[i];
	}



	// buffer_padding�� �����ϰ�, ����� ��ȯ�Ѵ�.
	subAlloc(padding);

	return output;
}





//�ѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤ�
//�� binarization
//�ѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤ�

/*
	global_fixed_thresholding
*/
Grayscale global_fixed_thresholding(Grayscale _Src, const int _T)
{
	Grayscale output = { NULL, _Src.width, _Src.height };
	output.pBuffer = (unsigned char*)myMalloc(sizeof(unsigned char) * output.width * output.height);


	int i;
	const int iLoop = (int)(output.width * output.height);
	for (i = 0; i < iLoop; ++i)
	{
		output.pBuffer[i] = _Src.pBuffer[i] >= _T ? 255 : 0;
	}


	return output;
}





//�ѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤ�
//�� histogram processing
//�ѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤ�

/*
	getLUT_with_straight

	[ �� ���� x, y��ǥ (x1, x2, y1, y2) ]
	�� ���� ���� �����Լ� �׷����� LUT�� �����ϰ� ��ȯ�Ѵ�.
	���ǿ�(x1���� x2������ ����)�� ��� �κ���, x��� ������ �����̴�.
*/
unsigned char* getLUT_with_straight(unsigned char x1, unsigned char x2, unsigned char y1, unsigned char y2)
{
	unsigned char* LUT = (unsigned char*)myMalloc(sizeof(unsigned char) * _8bit);


	int i;
	if (y1 == y2)
	{
		for (i = 0; i < _8bit; ++i)
		{
			LUT[i] = y1;
		}
	}


	if (x1 == x2)
	{
		for (i = 0; i < _8bit; ++i)
		{
			if (i <= x1)
				LUT[i] = y1;
			else
				LUT[i] = y2;
		}
	}
	else
	{
		float m = (float)(y1 - y2) / (float)(x1 - x2);
		float n = y1 - m * x1;

		if (x1 < x2)
		{
			for (i = 0; i < _8bit; ++i)
			{
				if (i <= x1)
				{
					LUT[i] = y1;
				}
				if (x1 < i && i < x2)
				{
					LUT[i] = (unsigned char)(m * (float)i + n);
				}
				if (x2 <= i)
				{
					LUT[i] = y2;
				}
			}
		}
		if (x2 < x1)
		{
			for (i = 0; i < _8bit; ++i)
			{
				if (i <= x2)
				{
					LUT[i] = y2;
				}
				if (x2 < i && i < x1)
				{
					LUT[i] = (unsigned char)(m * (float)i + n);
				}
				if (x1 <= i)
				{
					LUT[i] = y1;
				}
			}
		}
	}


	return LUT;
}





/*
	getLUT_invert

	[ void ]
	create inverted LUT
	"LUT(x) = 255 - x"
*/
unsigned char* getLUT_invert()
{
	unsigned char* LUT = (unsigned char*)myMalloc(sizeof(unsigned char) * _8bit);

	int i;
	for (i = 0; i < _8bit; ++i)
	{
		LUT[255 - i] = i;
	}

	return LUT;
}





/*
	[ ���� ���� ]
	���� ���� ���� ����(����ȭ)���� ���� histogram data�� �����ϰ�, �̸� ��ȯ�Ѵ�.
	���� ���� _Src�� buffer�� �������� �ʴ´�.
*/
uint32_t* extractHistogram(const Grayscale _src)
{
	uint32_t* histogram = (uint32_t*)myCalloc(sizeof(uint32_t) * _8bit);

	const size_t TOTAL_PIXELS = _src.width * _src.height;
	for (size_t i = 0; i < TOTAL_PIXELS; ++i)
	{
		++histogram[_src.pBuffer[i]];
	}
	return histogram;
}





/*
	accumulateArray
*/
uint32_t* accumulateArray(const uint32_t* _buffer, size_t _elementCount)
{
	uint32_t* accumulatedBuffer = (uint32_t*)myCalloc(sizeof(uint32_t) * _elementCount);

	size_t i, j;
	for (i = 0; i < _elementCount; ++i)
	{
		for (j = 0; j <= i; ++j)
		{
			accumulatedBuffer[i] += _buffer[j];
		}
	}
	return accumulatedBuffer;
}





/*
	createHistogramImage

	[0, 256]�� ����ȭ�� ������׷��� �ް�, 256*256�� �������� ��ȯ�Ѵ�.
*/
Grayscale createHistogramImage(const uint32_t* _histogram)
{
	size_t elementCount = _8bit * _8bit;
	Grayscale output = { (uint8_t*)myMalloc(sizeof(uint8_t) * elementCount), _8bit, _8bit };

	memset(output.pBuffer, 255, sizeof(uint8_t) * elementCount);

	for (size_t y = 0; y < output.height; ++y)
	{
		for (size_t x = 0; x < _histogram[y]; ++x)
		{
			output.pBuffer[(255 - x) * output.width + y] = 0;
		}
	}

	return output;
}





/*
	[ ���� ���� ]
	���� ���� ���� histogram image�� �����ϰ�, Grayscale�� ��ü�� ��ȯ�Ѵ�.
	���� ���� _Src�� buffer�� �������� �ʴ´�.
*/
Grayscale getHistogram_with_grayscale(Grayscale _Src)
{
	int* hist1DRaw = extractHistogram(_Src);

	unsigned char* hist1DCooked = (unsigned char*)normalizeArray(hist1DRaw, _8bit, 0, 255, "unsigned char");

	Grayscale hist_Src = { NULL, _8bit, _8bit };
	hist_Src.pBuffer = convert_histCooked_1Dto2D(hist1DCooked);

	return hist_Src;
}





/*
	[ ���� ���� ] . [ LUT ]
	���� ������ LUT�� ���� equalizing�ϰ�, Grayscale�� ��ü�� ��ȯ�Ѵ�.
	_Src�� buffer�� _LUT�� �������� �ʴ´�.
*/
Grayscale equalize_with_grayscale(Grayscale _Src, unsigned char* _LUT)
{
	Grayscale EQ_Src = { NULL, _Src.WIDTH, _Src.HEIGHT };
	EQ_Src.pBuffer = (unsigned char*)myMalloc(sizeof(unsigned char), EQ_Src.WIDTH * EQ_Src.HEIGHT);


	int i, iLoop = (int)(EQ_Src.WIDTH * EQ_Src.HEIGHT);
	for (i = 0; i < iLoop; ++i)
	{
		// WARNING : buffer overrun
		// compiler�� �ν��� ���ؼ� ������ ������ ����� �� �� �ִ�.
		// index������ Ȯ���ϹǷ�, ������ ��.
		EQ_Src.pBuffer[i] = _LUT[_Src.pBuffer[i]];
	}


	return EQ_Src;
}



/*
	applyLUTtoGrayscale
*/
Grayscale applyLUTtoGrayscale(const Grayscale _src, const uint32_t* _LUT)
{
	size_t elementCount = _8bit * _8bit;
	Grayscale dst = { (uint8_t*)myMalloc(sizeof(uint8_t) * elementCount), _src.width, _src.height};

	for (int i = 0; i < elementCount; ++i)
	{
		dst.pBuffer[i] = _LUT[_src.pBuffer[i]];
	}

	return dst;
}





/*
	[ ���� ���� ]
	���� ������ histogram data�� ���� LUT�� �����ϰ� ��ȯ�Ѵ�.
	���� ���� _Src�� buffer�� �������� �ʴ´�.
*/
unsigned char* getLUT_with_grayscale(Grayscale _Src)
{
	int* hist1D = extractHistogram(_Src);

	hist1D = accumulateArray(hist1D, _8bit);

	unsigned char* LUT_Src = (unsigned char*)normalizeArray(hist1D, _8bit, 0, 255, "unsigned char");

	return LUT_Src;
}





//�ѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤ�
//�� geometric processing
//�ѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤ�

/*
	shift

	[ input ] . [�̵��� x��ǥ��] . [y��ǥ��]
	���� _Src�� buffer�� �������� �ʴ´�.
*/
Grayscale shift(const Grayscale _Src, const int x, const int y)
{
	Grayscale Dst = { NULL, _Src.width, _Src.height };
	Dst.pBuffer = (unsigned char*)myMalloc(sizeof(unsigned char) * Dst.width * Dst.height);



	int i_begin, i_end, j_begin, j_end;
	if (x >= 0)
	{
		i_begin = 0;
		i_end = (int)Dst.width - x;
	}
	else
	{
		i_begin = -x;
		i_end = (int)Dst.width;
	}
	if (y >= 0)
	{
		j_begin = 0;
		j_end = (int)Dst.height - y;
	}
	else
	{
		j_begin = -y;
		j_end = (int)Dst.height;
	}

	int j, i;
	for (j = j_begin; j < j_end; ++j)
	{
		for (i = i_begin; i < i_end; ++i)
		{
			Dst.pBuffer[(int)Dst.width * (j + y) + (i + x)] = _Src.pBuffer[(int)_Src.width * j + i];
		}
	}

	return Dst;
}





/*
	rotation_bilinear_interpolation

	[ input ] . [ pos(clockwise) . neg(counter-clockwise) ]
	���� ������ �߽�(float)�� �������� ȸ���Ѵ�.
	���� _Src�� buffer�� �������� �ʴ´�.
	bilinear interpolation, reverse mapping.
*/
Grayscale rotation_bilinear_interpolation(Grayscale _Src, int _degree)
{
	// ��ǥ ����ȯ ���������� ���� ������ ���� input�� index������ ��� �Ҵ���� ���� ȭ�Ұ��� ���� �� ������,
	// ���� ó�� 1������ ������ �켱 ����ϰ� 2���� 3������ �ٽ� �ɷ����� �ᱹ �̷а� ���� ����� �����Ѵ�.
	// �д� �� ��ü�� ������ �ȴٸ�, input buffer�� �����¿� �� ���� �÷��� input ��� ����ϸ� �ȴ�.



	// ��� ��ü ����.
	Grayscale Dst = { NULL, _Src.width, _Src.height };
	Dst.pBuffer = (unsigned char*)myCalloc(sizeof(unsigned char) * Dst.width * Dst.height);



	// rotation�� ����� �����.
	const float radian = (float)(_degree % 360) * (float)M_PI / 180.f;
	const float COS = cosf(radian);
	const float SIN = sinf(radian);

	// ��ǥ ����ȯ ���꿡���� ��� ����.
	const float R = 0.0001f;



	// Dst �߽���. ( == _Src �߽���.)
	const float Xcenter = ((float)_Src.width - 1.f) / 2.f;
	const float Ycenter = ((float)_Src.height - 1.f) / 2.f;



	// Dst ���� ��ǥ.
	int Xdst, Ydst;

	// _Src ���� ��ǥ. (Dst ��ǥ�� ����ȯ�� ���� ���Ѵ�.)
	float Xsrc, Ysrc;



	// ���� ����ġ ����/��Ÿ.
	float a; // alpha (x-axis)
	float b; // beta (y-axis)

	// �������� ������ �ӽ� ����.
	float TEMP;



	// rotation with bilinear interpolation.
	for (Ydst = 0; Ydst < Dst.height; ++Ydst)
	{
		for (Xdst = 0; Xdst < Dst.width; ++Xdst)
		{

			// ����ȯ�� ���� _Src��ǥ�� ���Ѵ�.
			Xsrc = (Xdst - Xcenter) * COS + (Ydst - Ycenter) * SIN + Xcenter;
			Ysrc = -(Xdst - Xcenter) * SIN + (Ydst - Ycenter) * COS + Ycenter;



			// ���� ó�� 1 : Dst��ǥ�� ����ȯ�� _Src��ǥ�� index������ ����ٸ�, ���� loop�� �ǳʶڴ�.
			// (�Ҽ��� �Ϻΰ� �߸� ����� �����Ͽ� ������ �߻��ϹǷ�, ��� ���� R���� �����Ѵ�.)
			// (�� ������ ���ǹ��� ������ ������, ���� ����� �̷л� ������ ���ÿ� index������ �ش�ǰ� buffer�� �����ڸ��� �� ����� �� ������ �����Ѵ�.)
			if (-R > Xsrc || (float)(_Src.width - 1) + R < Xsrc || -R > Ysrc || (float)(_Src.height - 1) + R < Ysrc)
				continue;



			// ������ ���� _Src�� ���� ��ǥ�� ���� ����ġ alpha/beta���� ���Ѵ�.
			a = Xsrc - (int)Xsrc;
			b = Ysrc - (int)Ysrc;



			// ���� ó���� ���� �������� �ӽ� �����Ѵ�.
			// ���� ó�� 2���� 3���� linear interpolation(���� ����)�� �������� �߻��� �� �ִ� �����̹Ƿ�, �ذ��ؾ� �Ѵ�.
			TEMP =
				(1.f - a) * (1.f - b) * (float)_Src.pBuffer[(int)_Src.width * (int)Ysrc + (int)Xsrc] +
				a * (1.f - b) * (float)_Src.pBuffer[(int)_Src.width * (int)Ysrc + ((int)Xsrc + 1)] +
				(1.f - a) * b * (float)_Src.pBuffer[(int)_Src.width * ((int)Ysrc + 1) + (int)Xsrc] +
				a * b * (float)_Src.pBuffer[(int)_Src.width * ((int)Ysrc + 1) + ((int)Xsrc + 1)];



			// ���� ó�� 2 : �ʰ�. (����ȭ�Ұ� ��� 0 �Ǵ� 255�� ���� �������� ���� ������ ���� �߻��� �� ������, �̿ܿ��� ����� �߻��� �� ����.)
			if (255.f < TEMP)
				TEMP = 255.f;
			if (0.f > TEMP)
				TEMP = 0.f;



			// ���� ó�� 3 : �ݿø� + ���� ���������� ���� ó��.
			// (������ �ʿ� ���� 90�� ������ ȸ���� ������ ��ħ���ν� 0.001 ������ ������ �߻��Ѵ�. e.g.[lena������ +90�� ȸ�� : ���� 4915��])
			if (TEMP - (float)((int)TEMP) >= 0.5f)
				Dst.pBuffer[(int)Dst.width * Ydst + Xdst] = (unsigned char)TEMP + 1;
			else
				Dst.pBuffer[(int)Dst.width * Ydst + Xdst] = (unsigned char)TEMP;
		}
	}



	return Dst;
}





/*
	rotation_nearest_neighbor_interpolation

	[ input ] . [ pos(clockwise) . neg(counter-clockwise) ]
	���� ������ �߽�(float)�� �������� ȸ���Ѵ�.
	���� _Src�� buffer�� �������� �ʴ´�.
	nearest neighbor interpolation, reverse mapping.
*/
Grayscale rotation_nearest_neighbor_interpolation(Grayscale _Src, int _degree)
{
	// ��� ��ü ����.
	Grayscale Dst = { NULL, _Src.width, _Src.height };
	Dst.pBuffer = (unsigned char*)myCalloc(sizeof(unsigned char) * Dst.width * Dst.height);



	// rotation�� ����� �����.
	const float radian = (float)(_degree % 360) * (float)M_PI / 180.f;
	const float COS = cosf(radian);
	const float SIN = sinf(radian);

	// ��ǥ ����ȯ ���꿡���� ��� ����.
	const float R = 0.0001f;



	// Dst �߽���. ( == _Src �߽���.)
	const float Xcenter = ((float)_Src.width - 1.f) / 2.f;
	const float Ycenter = ((float)_Src.height - 1.f) / 2.f;



	// Dst ���� ��ǥ.
	int Xdst, Ydst;

	// _Src ���� ��ǥ. (Dst ��ǥ�� ����ȯ�� ���� ���Ѵ�.)
	float Xsrc, Ysrc;



	// ���� ����ġ ����/��Ÿ.
	float a; // alpha (x-axis)
	float b; // beta (y-axis)



	// rotation with nearest neighbor interpolation.
	for (Ydst = 0; Ydst < Dst.height; ++Ydst)
	{
		for (Xdst = 0; Xdst < Dst.width; ++Xdst)
		{

			// ����ȯ�� ���� _Src��ǥ�� ���Ѵ�.
			Xsrc = (Xdst - Xcenter) * COS + (Ydst - Ycenter) * SIN + Xcenter;
			Ysrc = -(Xdst - Xcenter) * SIN + (Ydst - Ycenter) * COS + Ycenter;



			// ���� ó�� 1 : Dst��ǥ�� ����ȯ�� _Src��ǥ�� index������ ����ٸ�, ���� loop�� �ǳʶڴ�.
			// (�Ҽ��� �Ϻΰ� �߸� ����� �����Ͽ� ������ �߻��ϹǷ�, ��� ���� R���� �����Ѵ�.)
			// (�� ������ ���ǹ��� ������ ������, ���� ����� �̷л� ������ ���ÿ� index������ �ش�ǰ� buffer�� �����ڸ��� �� ����� �� ������ �����Ѵ�.)
			if (-R > Xsrc || (float)(_Src.width - 1) + R < Xsrc || -R > Ysrc || (float)(_Src.height - 1) + R < Ysrc)
				continue;



			// ������ ���� _Src�� ���� ��ǥ�� ���� ����ġ alpha/beta���� ���Ѵ�.
			a = Xsrc - (int)Xsrc;
			b = Ysrc - (int)Ysrc;



			// ���� ȭ�Ұ��� �״�� �������� ���� �����̹Ƿ�, ���� ó���� ����.
			if (0.5f > a)
			{
				if (0.5f > b)
				{
					Dst.pBuffer[(int)Dst.width * Ydst + Xdst] = _Src.pBuffer[(int)_Src.width * (int)Ysrc + (int)Xsrc];
				}
				else
				{
					Dst.pBuffer[(int)Dst.width * Ydst + Xdst] = _Src.pBuffer[(int)_Src.width * ((int)Ysrc + 1) + (int)Xsrc];
				}
			}
			else
			{
				if (0.5f > b)
				{
					Dst.pBuffer[(int)Dst.width * Ydst + Xdst] = _Src.pBuffer[(int)_Src.width * (int)Ysrc + ((int)Xsrc + 1)];
				}
				else
				{
					Dst.pBuffer[(int)Dst.width * Ydst + Xdst] = _Src.pBuffer[(int)_Src.width * ((int)Ysrc + 1) + ((int)Xsrc + 1)];
				}
			}
		}
	}



	return Dst;
}





/*
	zoomIn_bilinear_interpolation

	[ input ] . [ output.width ] . [ output.height ]
	���� _Src�� buffer�� �������� �ʴ´�.
	bilinear interpolation, reverse mapping.
*/
Grayscale zoomIn_bilinear_interpolation(const Grayscale _Src, const size_t _DstWidth, const size_t _DstHeight)
{
	/*
		_Src�� bilinear interportion�� �����ϱ� ���ؼ���, input.pbuffer�� ���/�ϴ� Line�� �� �پ� �� �ʿ��ϴ�.
		_Src.width = _Src.width + 1
		_Src.height = _Src.height + 1
		�Է� _Src.pbuffer�� input.pbuffer�� �����ϰ�, �߰� ������ �̿��� ȭ���� ������ ��ü�Ѵ�.
	*/
	Grayscale input = { NULL, _Src.width + 1, _Src.height + 1 };
	input.pBuffer = (unsigned char*)myMalloc(sizeof(unsigned char) * input.width * input.height);
	int j, i;
	for (j = 0; j < _Src.height; ++j)
	{
		for (i = 0; i < _Src.width; ++i)
		{
			input.pBuffer[input.width * j + i] = _Src.pBuffer[_Src.width * j + i];
		}
	}
	for (j = 0; j < input.height - 1; ++j)
	{
		input.pBuffer[input.width * j + (input.width - 1)] = input.pBuffer[input.width * j + (input.width - 2)];
	}
	for (i = 0; i < input.width; ++i)
	{
		input.pBuffer[input.width * (input.height - 1) + i] = input.pBuffer[input.width * (input.height - 2) + i];
	}
	// ���� _Src.pbuffer ��ſ� input.pbuffer�� �����Ͽ� �����ϸ� �ȴ�.



	// output ����.
	Grayscale output = { NULL, _DstWidth, _DstHeight };
	output.pBuffer = (unsigned char*)myMalloc(sizeof(unsigned char) * output.width * output.height);



	// bilinear interpolation.
	// ��ǥ (i, j)�� output�� �������� �Ѵ�.
	const float Xpct = (float)_Src.width / (float)_DstWidth;
	const float Ypct = (float)_Src.height / (float)_DstHeight;

	float a; // alpha (x-axis)
	float b; // beta (y-axis)

	for (j = 0; j < output.height; ++j)
	{
		b = ((float)j * Ypct) - (int)((float)j * Ypct);

		for (i = 0; i < output.width; ++i)
		{
			a = ((float)i * Xpct) - (int)((float)i * Xpct);
			

			output.pBuffer[(int)output.width * j + i] = (unsigned char)(
				(1.f - a) * (1.f - b) * (float)input.pBuffer[(int)input.width * (int)((float)j * Ypct) + (int)((float)i * Xpct)] +
				a * (1.f - b) * (float)input.pBuffer[(int)input.width * (int)((float)j * Ypct) + ((int)((float)i * Xpct) + 1)] +
				(1.f - a) * b * (float)input.pBuffer[(int)input.width * ((int)((float)j * Ypct) + 1) + (int)((float)i * Xpct)] +
				a * b * (float)input.pBuffer[(int)input.width * ((int)((float)j * Ypct) + 1) + ((int)((float)i * Xpct) + 1)]);
		}
	}



	// _Src.pbuffer ��� ����� input.pbuffer�� �����ϰ�, output�� ��ȯ�Ѵ�.
	subAlloc(input.pBuffer);
	return output;
}





/*
	zoomIn_nearest_neighbor_interpolation

	[ input ] . [ output.width ] . [ output.height ]
	���� _Src�� buffer�� �������� �ʴ´�.
	nearest neighbor interpolation, reverse mapping.
*/
Grayscale zoomIn_nearest_neighbor_interpolation(const Grayscale _Src, const size_t _DstWidth, const size_t _DstHeight)
{
	/*
		_Src�� bilinear interportion�� �����ϱ� ���ؼ���, input.pbuffer�� ���/�ϴ� Line�� �� �پ� �� �ʿ��ϴ�. (��� ����� �� ���� �Ѵ�.)
	 
		�Է� ������ ���� ���̰� a, ��� ������ ���� ���̰� b�� �� ��,
		Xpct = a/b
		"(b - 1) * Xpct >= (a - 1) + 0.5"
		�� ���� ������Ű�� �ڿ��� �� (a, b)�� �����ϴ°�??
		���� -> "b >= 2 * a"
		������ ���� �����ϸ�, ���� ���� ���� ���������̹Ƿ�, �ᱹ Line�� �ϳ��� �߰��ؾ� �Ѵ�.
	 
		_Src.width -> _Src.width + 1
		_Src.height -> _Src.height + 1
		�Է� _Src.pbuffer�� input.pbuffer�� �����ϰ�, �߰� ������ �̿��� ȭ���� ������ ��ü�Ѵ�.
	*/
	Grayscale input = { NULL, _Src.width + 1, _Src.height + 1 };
	input.pBuffer = (unsigned char*)myMalloc(sizeof(unsigned char) * input.width * input.height);
	int j, i;
	for (j = 0; j < _Src.height; ++j)
	{
		for (i = 0; i < _Src.width; ++i)
		{
			input.pBuffer[input.width * j + i] = _Src.pBuffer[_Src.width * j + i];
		}
	}
	for (j = 0; j < input.height - 1; ++j)
	{
		input.pBuffer[input.width * j + (input.width - 1)] = input.pBuffer[input.width * j + (input.width - 2)];
	}
	for (i = 0; i < input.width; ++i)
	{
		input.pBuffer[input.width * (input.height - 1) + i] = input.pBuffer[input.width * (input.height - 2) + i];
	}
	// ���� _Src.pbuffer ��ſ� input.pbuffer�� �����Ͽ� �����ϸ� �ȴ�.



	// output ����.
	Grayscale output = { NULL, _DstWidth, _DstHeight };
	output.pBuffer = (unsigned char*)myMalloc(sizeof(unsigned char) * output.width * output.height);



	// nearest neighbor interpolation.
	// ��ǥ (i, j)�� output�� �������� �Ѵ�.
	const float Xpct = (float)_Src.width / (float)_DstWidth;
	const float Ypct = (float)_Src.height / (float)_DstHeight;

	float a; // alpha (x-axis)
	float b; // beta (y-axis)

	for (j = 0; j < output.height; ++j)
	{
		for (i = 0; i < output.width; ++i)
		{
			a = ((float)i * Xpct) - (int)((float)i * Xpct);
			b = ((float)j * Ypct) - (int)((float)j * Ypct);

			if (a <= 0.5f)
			{
				if (b <= 0.5f)
				{
					output.pBuffer[(int)output.width * j + i] = input.pBuffer[(int)input.width * (int)((float)j * Ypct) + (int)((float)i * Xpct)];
				}
				else
				{
					output.pBuffer[(int)output.width * j + i] = input.pBuffer[(int)input.width * ((int)((float)j * Ypct) + 1) + (int)((float)i * Xpct)];
				}
			}
			else
			{
				if (b <= 0.5f)
				{
					output.pBuffer[(int)output.width * j + i] = input.pBuffer[(int)input.width * (int)((float)j * Ypct) + ((int)((float)i * Xpct) + 1)];
				}
				else
				{
					output.pBuffer[(int)output.width * j + i] = input.pBuffer[(int)input.width * ((int)((float)j * Ypct) + 1) + ((int)((float)i * Xpct) + 1)];
				}
			}
		}
	}



	// _Src.pbuffer ��� ����� input.pbuffer�� �����ϰ�, output�� ��ȯ�Ѵ�.
	subAlloc(input.pBuffer);
	return output;
}





/*
	TEST_zoonInOut

	Xpct/Ypct�� 1 �̻��� �Ǵ� "���"�� ��쿡,
	��հ� ������ ��� �����ϸ�,
	Xpct/Ypct�� 1 �̸��� ���� �Ȱ��� ����ȴ�.
*/
Grayscale TEST_zoonInOut(const Grayscale _Src, const size_t _DstWidth, const size_t _DstHeight)
{
	/*
		���� : https://cho001.tistory.com/146

		11.22 ����
		1. 256*256 ������ 64*64�� ���
		2. 1�� ������ 256*256���� Ȯ��
		3. 1�� ������ 300*300���� Ȯ��
		4. ������ ������ N*N���� Ȯ��. (���� Ȯ��)

		5. ������ ������ ȸ��.



		Ȯ�뿡 ���ؼ�.
			������ ���, ������ ���.
			ä����� ���� Ȯ�� �����̴�.
			Ȯ�뿵���� idx�� ä��������,
			src������ ������ �� ���̴�.
			�̴� ������ ����̴�.
			��� �츮�� �ϰ� �ִ� �͵��̴�.


		4�� ���̰� 4�� �ø���
		������ ���;� �Ѵ�.
		�׷�����,
		Ȯ����� �Լ��� ������ ������ �Ź� �ްų�, �޾��� �� �����صξ�� �Ѵ�.
		�Ŀ�����Ʈ�� ���� �ְ� Ȯ������ص� �Ⱥ��ϴ°�, ó�� ������ ������ ������ �ֱ� �����̴�.


		������
			idx�� 21.3�̶��,
			idx21�� 70%�� idx22�� 30%�� �������� �ǰڴ�.
			y�൵ �Ȱ��� �����ϸ� �� �� ����.
			�̸� ����� ��������(bilinear interpolation)�̶�� �Ѵ�.
			���� ���� �ݿø��ؼ� �����ȼ�(nearless neighborhood???)�� �������� ����� �ִ�.


		����
		1.
		zoom-in/zoom-out �Լ��� global object�� ���Ͽ� origin�� ������ ������ �����ϵ�,
		���� ���� �׽� origin �������� �����ϰ� return�� ��.

		2.
		zoom-in/zoom-out �Լ��� ������ ���� ���� ��������, ������ �Լ��� �����ϰ� ����� ��.
		Ȯ��/��� �Ӹ� �ƴ϶�, ȸ�� ���� ������ ������ ����� �� �ֱ� �����̴�.
		// �ƴ� �� ����. �������� �� ��������. �׳� ������ �ܿ�����.
	*/



	// _Src�� bilinear interportion�� �����ϱ� ���ؼ���, 2Dbuffer�� ���/�ϴ� pixel�� �ϳ��� �� �ʿ��ϴ�.
	// _Src.width -> _Src.width + 1,  _Src.height -> _Src.height + 1
	// �� �߰� ������, �̿��� ȭ���� ������ ��ü�Ѵ�.
	// ��� ������,
	// �߰� ������ �̿��� ȭ���� ������ ��ü�ϴ� �� �Ȱ���,
	// ���� 10�� ����Ѵٸ�, �ּ� 10�� �̻��� �߰� ��ǥ�� �ʿ����ٵ�,,

	// Ȯ�뿡���� ������ ������ ���� ��������,
	// ��ҿ��� ������ ������ �ϱ� ���ؼ� ������ ����� ���ÿ� �Ϸ���, ���� ����������.
	// ���࿡,
	// width/height �� �ϳ��� ���, �ϳ��� Ȯ����??
	// ������� �������� ���ÿ� �����ұ�??

	// �����ϴٸ�, ��� ������ ��� ���� _Src.pbuffer�� �߰� ������ +1�� ������ų �� �ִ�.

	// �����ѵ�, ����/������ �����ؾ� �ϰ�, ���� ������/������ ȥ���� �� �����ϴ�.

	// �׳� ó�� �����Ѵ�� ���������� �� �ذ��ϵ�, if������ ���� ���� ÷������.
	// �翬�� �߰� ������ 1�� ������������, ������ idx�� ���ϸ� �׸����ڴ� �͵� if�������� �ΰ�.



	Grayscale input = { NULL, _Src.width + 1, _Src.height + 1 };
	input.pBuffer = (unsigned char*)myMalloc(sizeof(unsigned char) * input.width * input.height);
	int j, i;
	for (j = 0; j < _Src.height; ++j)
	{
		for (i = 0; i < _Src.width; ++i)
		{
			input.pBuffer[input.width * j + i] = _Src.pBuffer[_Src.width * j + i];
		}
	}
	for (j = 0; j < input.height - 1; ++j)
	{
		input.pBuffer[input.width * j + (input.width - 1)] = input.pBuffer[input.width * j + (input.width - 2)];
	}
	for (i = 0; i < input.width; ++i)
	{
		input.pBuffer[input.width * (input.height - 1) + i] = input.pBuffer[input.width * (input.height - 2) + i];
	}
	// ���� _Src.pbuffer ��ſ� input.pbuffer�� �����Ͽ� �����ϸ� �ȴ�.



	// output ����.
	Grayscale output = { NULL, _DstWidth, _DstHeight };
	output.pBuffer = (unsigned char*)myMalloc(sizeof(unsigned char) * output.width * output.height);



	// output ���� ��ǥ.
	int X_output;
	int Y_output;

	const float Xpct = (float)_Src.width / (float)_DstWidth;
	const float Ypct = (float)_Src.height / (float)_DstHeight;


	// output ���� ��ǥ�� ���� input ��ǥ.
	float X_input = 0;		// X_output * Xpct
	float Y_input = 0;		// Y_output * Ypct

	float a;			// alpha (x-axis)
	float b;			// beta (y-axis)


	float SUM;
	int COUNT;
	for (Y_output = 0; Y_output < output.height; ++Y_output)
	{
		for (X_output = 0; X_output < output.width; ++X_output)
		{
			SUM = 0, COUNT = 0;

			for (j = (int)Y_input; j <= (int)(Y_input + Ypct); ++j)
			{
				for (i = (int)X_input; i <= (int)(X_input + Xpct); ++i)
				{
					a = ((float)i * Xpct) - (int)((float)i * Xpct);
					b = ((float)j * Ypct) - (int)((float)j * Ypct);
				}
			}




			Y_input += Ypct;
			X_input += Xpct;
		}


		//for (j = 0; j < output.height; ++j)
		//{
		//	for (i = 0; i < output.width; ++i)
		//	{
		//		a = ((float)i * Xpct) - (int)((float)i * Xpct);
		//		b = ((float)j * Ypct) - (int)((float)j * Ypct);

		//		output.pbuffer[(int)output.width * j + i] = (unsigned char)(
		//			(1.f - a) * (1.f - b) * (float)input.pbuffer[(int)input.width * (int)((float)j * Ypct) + (int)((float)i * Xpct)] +
		//			a * (1.f - b) * (float)input.pbuffer[(int)input.width * (int)((float)j * Ypct) + ((int)((float)i * Xpct) + 1)] +
		//			(1.f - a) * b * (float)input.pbuffer[(int)input.width * ((int)((float)j * Ypct) + 1) + (int)((float)i * Xpct)] +
		//			a * b * (float)input.pbuffer[(int)input.width * ((int)((float)j * Ypct) + 1) + ((int)((float)i * Xpct) + 1)]);
		//	}
		//}
	}



	// _Src.pbuffer ��� ����� input.pbuffer�� �����ϰ�, output�� ��ȯ�Ѵ�.
	subAlloc(input.pBuffer);
	return output;
}





/*
	TEST_TEST_zoomInOut

	Ȯ����� ��� �����ϰ� �������̶� ��� ��� Ȱ��. ���̽��� reverse mapping. 1D�� ������.
*/
int* TEST_TEST_zoomInOut(unsigned char* _Src, const size_t _Src_width, const size_t _Dst_width)
{
	int* _Dst = (int*)myMalloc(sizeof(int) * _Dst_width);


	float a;

	const float Xpct = (float)_Src_width / (float)_Dst_width;

	int X_output;

	float X_input = 0;		// == X_output * Xpct

	float SUM;
	float COUNT;



	int i;
	for (X_output = 0; X_output < _Dst_width; ++X_output)
	{
		SUM = 0, COUNT = 0;
		for (i = (int)X_input; i <= (int)(X_input + Xpct) + 1; ++i)
		{
			if ((float)i > 1)
			{

			}

			if ((float)i == X_input)
			{
				_Dst[X_output] += _Src[i];
				++COUNT;
				continue;
			}

			if (1)
			{
				_Dst[X_output] += _Src[i];
				++COUNT;
			}

			if ((float)i > X_input + Xpct)
			{
				if ((float)i == X_input + Xpct)
				{
					continue;
				}
			}



			a = ((float)X_input) - (int)((float)X_input);

			_Src[i];



		}



		X_input += Xpct;
	}



	return _Dst;
}





inline float bilinear_interpolation(float* p0_0, float* p0_1, float* p1_0, float* p1_1, float* a, float* b)
{
	float V1 = (1.f - *a) * (1.f - *b) * *p0_0;
	float V2 = *a * (1.f - *b) * *p0_1;
	float V3 = (1.f - *a) * *b * *p1_0;
	float V4 = *a * *b * *p1_1;
	return V1 + V2 + V3 + V4;
}



























inline void selectionSort(uint8_t _arr[], int32_t _count) {
	int32_t i, j, min_idx;
	uint8_t temp;

	// �迭�� ��� ��Ҹ� ��ȸ
	for (i = 0; i < _count - 1; i++) {
		// ���� ��ġ���� �ּڰ��� �ε����� ã��
		min_idx = i;
		for (j = i + 1; j < _count; j++) {
			if (_arr[j] < _arr[min_idx]) {
				min_idx = j;
			}
		}

		// �ּڰ��� ���� ��ġ�� ��ȯ
		if (min_idx != i) {
			SWAP(_arr[i], _arr[min_idx], temp);
		}
	}
}




#endif // GRAYSCALE_H
















/*
�ѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤ�
�� note
�ѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤ�

*** �� �ּ���, [���α׷��־��2]���񿡼� ����ü ���� �� ���� �ִ��� �ñ��Ͽ�, ������ ���� ó���� ������ �˰����� ��Ŀ� ���� �з��� ���̴�.

*** ������ ���� ó���� ��з����� �������� �ۼ��� �ּ��̸�, �ŷ��� ���� ������, ������ ���� �ϼ����� �ʾҴ�.

***	ǥ�õ� ���� ����Ʈ��, ���� �Ϻ��� ���̾ ������� ���� ���̰ų�, ���� ������ ���� ���� ���̰ų�, ������ ������ ���� �Ǵ� �ڼ��� ������ ���� ���� ���̴�.

***	spatial filtering(���� ���͸�)�� ���ؼ��� image domain filtering(���� ���� ���͸�)�� �ٷ��, frequency domain filtering(���ļ� ���� ���͸�)�� �ٷ��� �ʾҴ�.
	image processing(���� ó�� ���)  ->  spatial domain processing(���� ������ ����), frequency domain processing(���ļ� ������ ����)

***	�� ����, ����ó���� ���� ���� �����̸� ��κ��� ���� ������ �����Ǿ� ������, ��ó�� �����. �ٵ� ���� �𸥴�.

***	mask Ȥ�� kernel�� ��ģ pixel���� ��� ���ϴ����� ������ �� ��. �̴� filter�� ������ �к��ϴµ� �߿��� ������ �Ѵ�.

***	������ filter�� ���� ��� ���⵵�� �˾Ƽ� �Ǵ��� ��.

*** �����а� ���� ������ ������ ��ƴٸ�, < Histogram Modification > ��Ʈ�� �ִ� Ȯ������ ���� ����/�̻� �׷����� �����Ͽ� kernel�� ���� ���� �� �ִ�.
	HPF�� �̻�̺� �׷����� ���ð� �ִ�. ���� : https://everyday-image-processing.tistory.com/142
	������ �� filter���� kernel�� ������ ���� �����Ƿ�, �ǽ��� ���� kernel���� �񱳰� �����ϴٸ�, ������ ������ �����Ѵ�.
	������ �ʿ信 ���� customizing �����ϴ� ���� �Ұ����� ���̴�.
	���� ���� ���ߴ�.







	< digital image processing > (������ ���� ó�� / ������ ȭ�� ó��)
		// ���� : https://typoon.tistory.com/m/14

		������ ���� ó�� ����� ���� �з�
			Image Enhancement (���� ����)
			Image Restoration (���� ����)
			Image Transformation (���� ��ȯ)
			Image Analysis (���� �м�)
			Image Understanding (���� �ν�)
			Image Compression (���� ����)

		������ ���� ó�� �˰����� ���� �з�
			Pixel Point Processing (ȭ�� �� ó��, ���� �ȼ� ó��)
			Pixel Area(Group) Processing (ȭ�� ���� ó��, �ȼ� �׷� ó��)
			Geometric Processing (�������� ó��)
			Frame Processing (������ ó��)







	< interpolation > (����, ������)
	@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ ��������.

		// 







	< Geometric Processing > (�������� ó��)

			�� mapping (���)
				// ���� : https://cho001.tistory.com/146
				// ���� : C:\Users\bibib\OneDrive\���� ȭ��\�ڵ�����\13����_����\RESULT.PNG

					�� forward mapping (������ ���) : �Է� ������ ��ǥ�� �������� ó���Ѵ�.

					�� reverse mapping (������ ���) : ��� ������ ��ǥ�� �������� ó���Ѵ�.

						�Է�/����� ũ�Ⱑ ���ٸ� ���������, �׷��� �ʴٸ� hole(Ȧ)�̳� overlap(������)�� ������ �߻��� �� �ִ�.



					�� rotation (ȸ��)
						// ���� : https://cho001.tistory.com/146
						// ���� : https://m.blog.naver.com/PostView.naver?isHttpsRedirect=true&blogId=junhyuk7272&logNo=220140840595
						// ���� : https://color-change.tistory.com/54
						// ������ ������ ������, ���ĸ� ������.

							ȸ���� �Է� ������ ��� ȭ�Ҹ�, �Է��� ���ϴ� ���� ��ǥ�� �������� ���ϴ� ������ŭ ��� ȭ�ҿ� ���� ȸ�� ��ȯ�ϴ� ���� ���Ѵ�.

							�Է� ��ǥ�� (X, Y), ȸ�� ��ȯ�� ��ǥ�� (X', Y')�̰�
							���� ��ǥ�� (0, 0)�̶� �ϸ�,

								�� forward mapping	  :	X' = Xcos(��) - Ysin(��)
														Y' = Xsin(��) + Ycos(��)

								�� reverse mapping	  :	X = X'cos(��) + Y'sin(��)
														Y = -X'sin(��) + Y'cos(��)


							���� ��ǥ�� (Xctr, Yctr)��� �ϸ�,

								�� forward mapping	  :	X' = (X - Xctr)cos(��) - (Y - Yctr)sin(��) + Xctr
														Y' = (X - Xctr)sin(��) + (Y - Yctr)cos(��) + Yctr

								�� reverse mapping	  :	X = (X' - Xctr)cos(��) + (Y' - Yctr)sin(��) + Xctr
														Y = -(X' - Xctr)sin(��) + (Y' - Yctr)cos(��) + Yctr

															

					�� ȸ�� ��ȯ ���� ������ ���Ͽ�. (rotation() �Լ�)

							��� : float������ ������ ���, 90�� ������ ȸ�� ��ȯ���� 0.0000076 ~ 0.0000077 ������ ������ �����. double������ ������ ���, ������ �� �۾��� ���̴�.
									�̷��� ���� ������ ������ �߻����״�.


							rotation() �Լ��� reverse mapping���� output��ǥ���� �̿� �����Ǵ� input ����ȯ ��ǥ���� �����ϴ� ������� �����Ǿ���, �缱�� ������ �����Ѵ�.
							output��ǥ�� ����ȯ�� input������ ��ǥ�� index������ ����ٸ�, �ش� output��ǥ�� �ǳʶڴ�.

							�� index������ ��� �� �ǳʶڴٴ� ���� ó������ ������ �����.



							256x256 ���󿡼� x�� y�� ������ ������ ����.
								0 <= x <= 255
								0 <= y <= 255
							�߽����� ��Ȯ�� 127.5�� ��� 90�� ������ ȸ���� ��
							�缱�� ������ ����/��Ÿ ���� �׻� 0���� ������ �ʿ䰡 ����� �ϴµ�,
							65536�� �� 82���� pixel�� input�� index������ �����. ��, �����ڸ� �Ϻκ��� ��µ��� �ʴ´�.

							������ �Ʒ��� ����.
								��	math.h�� ������, �ﰢ�Լ� ���� ����/��ȯ���� ���� ���� �ƴϹǷ� ������ �����Ѵ�.
									ó������ ������ �����ϴ� ����� ������ ������, ���� ��� ���� �翬�� ������ �����Ѵ�.
									�� ������ ���ǹ��� ������ ������, ���� ����� �̷л� ���������� �� ������ �߻��Ѵ�.

							�ٺ����� ������ ������ �ذ��ϴ� ������� �� ������ �ִ�. �̰� �ܿ��� ��ǻ� �Ұ����ϴ�.
								��	����� ���� calculation module�� �ʿ��ϴ�. �̴� ����� ������ ���� ����ϵ��� ������ ������ ���� ���̴�.


							������ �ذ����� �ʴ´ٸ�, ���� ó���� ���� �Ѵ�.
								��	x�� y�� ������ ������ ���� �Ѵ�.
										R = 0.00001f
										-R <= x <= 255 + R
										-R <= y <= 255 + R
									���� input�� �����¿� �� ���� �߰��Ͽ� �����Ѵ�.
									// �߰��Ͽ� �������� �ʴ´ٰ� �ص�, �������� index������ ����� pixel�� ���� ������ R����ŭ ���� ���̴�.
									// �׷���, �̰͵� ������ �޸� ħ��(buffer overflow)�̴�.
									// �Ҵ�� memory���� ����ϸ�, �Ҵ���� ���� memory�� ������� �ʴ� ���� �ſ� �翬�ϴ�.







	< Pixel Point Processing > (ȭ�� �� ó��)  ==  ���� �ȼ� ó��

		�̿� �ȼ����� ����������
		�Է� ������ �� �ȼ� ���� �������� �Լ��� ���� ���ο� ������ ��ȯ�ϰ�, ��� ������ ������ ��ġ�� �����ϴ� �����̴�.

		ȭ�� ó������ �� ��� ȭ�Ұ��� �ش� �Է� ȭ�Ұ����� �����Ѵ�.
		�̰��� ��� ȭ���� ���� ������ ��, �̿��� ȭ�ҵ��� ���� Ȱ���ϴ� ���� ���͸��� �񱳵ȴ�.

		// ���� ���� ó���� ���� �´ٸ�, mask ����� [1*1]�̴�.

			�� �������, ��������, �׷��̽����� ��ȯ, Global Fixed Thresholding(���� ���� ����ȭ), Histogram Modification(������׷� ����), Global Histogram Processing(������ ������׷� ó��) ��

			�� e.g.
				darken(f-a) lighten(f+a) invert(255-f) low-contrast(f/2) high-contrast(f*2) gray(0.3f_R + 0.6f_G + 0.1f_B)







	< Pixel Area(Group) Processing > (ȭ�� ���� ó��, �ȼ� �׷� ó��)

		����ȭ�ҵ��� ���� �����Ͽ� ȭ���� ���� �����ϴ� ó���̴�.

		�� Local Adaptive Thresholding(���� ���� ����ȭ), Local Histogram Processing(������ ������׷� ó��), Spatial Filtering (���� ���͸�) ��







	< Padding > (�е�)
		// ���� : https://ardino.tistory.com/40
		// ���� : https://ardino.tistory.com/41

		// ���� ó���� �ϱ� ���� �� �˾ƾ� �� �����̴�.

		// �� �̸� �е��� ������ ������ �Ҹ��̸�, mask����� �������� �ƴ� ���̾Ƹ��, ���ڰ� ������ �Ѵ� �����ϸ� ���ذ� �� ���̴�.


		padding (�е�) : �����ڸ� ȭ�Ҹ� filtering�ϱ� ���ؼ�, �Է� ������ �ܰ��� �ӽ� ������ ���� �߰��ϴ� ��.
							��ŭ�� ������ ��� ���� ����ϴ����� ���� padding�� ������ �����ȴ�.
							// ȭ���� ���� ��������, Ư�� ȭ�Ұ� mask�� �� �� ���ԵǾ������� ���Ѵ�.
							// �����ڸ� ȭ�Ҵ� ���� ó���� �Ұ����ϰ�, padding�� ���� ���� �� ��� ������ SIZE�� �����Ѵ�.
							// padding�� �̸� �ذ��ϱ� ���� ����̸�, ��� ���� �翬�� �����ؾ� �Ѵ�.
							// ���� : �Ϲ������� �Ǽ����̸�, ���� �Ǽ����� kernel���� convolution������ ����ȴ�.


		Stride (��Ʈ���̵�, ����) : filtering���� mask�� �����̴� �Ÿ�.
									���͸��� ���͸� �̹����� ���� �����̸鼭 �Ѵ�.
									�̶� ���Ͱ� �ѹ��� �����̴� �Ÿ��� ����, ���� ��Ʈ���̵�(stride)��� �θ���.
									mask�� �� ĭ�� �����̸� ��Ʈ���̵� ���� 1�� �ǰ�, �� ĭ�� �����̸� 2�� �ȴ�.
									// stride ���� ��� ������ SIZE�� �����Ѵ�.



		�� padding�� �߰� ������ ũ�⿡ ���� �з�

			�� Valid Padding : padding�� �������� �ʴ� ��.

			�� Full Padding : �Է� ������ ��� ȭ���� ���� �������� �����ϵ��� padding�ϴ� ��.
								// ��� ������ SIZE�� �����Ѵ�.
								// �������� �Է� ���� ���� �������� �ƴ� mask�� full padding�� ����� ��, ��� ������ �������� �ƴϰ� �Ǵ� �κ��� ���� �𸣰ڴ�.
								// �Ƹ��� ���������� ����� ���ؼ� �ܰ��� �� ������ �߰� ������ �ϸ� �� �� ����.

			�� Same Padding : �Է� ����� ��� ������ ũ�Ⱑ ������ padding�ϴ� ��.



		�� padding�� �߰� ������ ���� ���� �з�
			// ���� : https://wiserloner.tistory.com/845
			// ���� : https://cafepurple.tistory.com/54

			��ǥ���� �� ���� ����� ������, filter���� �ٸ� �� �ִ�.
			���� ����Ʈ�� openCV�� ���ǵǾ��ִ� padding ���� ����� �ִ�.

			1.	Zero-Padding (0-padding, ���� �е�)
				����������, ��� ������ �����ڸ� �����Ͱ� ����/�ҽǵȴ�.

			2.	�����ڸ� ������ padding�Ѵ�.







	< Spatial Filtering > (���� ���͸�)
		// ���� : https://www.geeksforgeeks.org/spatial-filtering-and-its-types/

		Filter (����) : �Է� ��ȣ�� �Ϻ� ������ �����ϰų� �Ϻ� Ư���� �����ϱ� ���� ����� �ϳ��� �ý����̴�.

		���� ���͸��̶�, ���� ��ȣ�� ���� ���� ����(Spatial Domain)������ ���� ó���� �ǹ��Ѵ�.

		// ���͸��� ���� ��� ���ļ� �������� �ٷ�� �� �״�� ����Ѵ�.
		// "��������(spatial domain)"�̶� Ī�ϴ� ������ "���ļ� ����(frequency domain)"�� ���̸� �α� ���ؼ��̴�.
		// ȭ�ҵ��� �����ϴ� 2���� �������� ���͸��� �����Ѵٴ� �ǹ��̴�.

		// "spatial filtering"��, "Local Histogram Processing"�� ���� ������ �ٸ� �� ���� �����ϴ�.


		Mask (����ũ) : filtering�� ������ �����ϴ� 2���� ����̴�.
						mask�� ����Ǵ� ������ ���´�, filtering�� ����� �����ϰ� filter�� ������ �����Ѵ�.
						// kernel(Ŀ��), window(������), template(���ø�)�̶���� �θ���, mask ��ü�� filter��� �θ��⵵ �Ѵ�.
						// Ȥ�� ���� ������ Ư�� ��ġ�� ���� filtering�� ����Ǵ� ������ mask�� �ϱ⵵ �Ѵ�.
						// �̶��� �����̶�, { (filtering�ϱ� ���� ���� ������ �Ϻκ�), (mask), (������ ����) }�̴�.


		Kernel (Ŀ��) : linear spatial filtering(���� ���� ���͸�)�� convolution���꿡 ���Ǵ� ����ġ ����̴�.
						// ���� : padding�� ����� �Ǽ��� buffer�� convolution������ ����ǹǷ�, kernel�� �Ǽ������� �Ѵ�.



		filter�� �������� ���� �з�
			Linear Spatial Filter (���� ���� ����) : convolution������ ����ϴ� filter�̴�.
			Nonlinear Spatial Filter (���� ���� ����) : convolution������ ������� �ʰ�, ���� ���� �ٸ� ������ ����ϴ� filter�̴�.

		�� �ٸ� �Ϲ��� �з�
			smoothing spatial filter(������ ���� ����)  ->  smoothing linear filter(������ ���� ����), order-statistic filter(����-��� ����)

		���� ���� ���Ϳ� ����-��� ���ʹ� ���ٰ� ���� �ȴ�.



		***	1.	��-trimmed Mean Filter(��-���� ��հ� ����)�� ��հ� ���Ϳ� �߰��� ������ ����� �����ϹǷ�, �� ���͸� ������ �� �ִ�.
		***	2.	linear filter�� ����ġ ����� kernel�� ������ ���ؼ��� �з��ǹǷ�,
		***		kernel�� ���ڷ� �޴� �� �ϳ��� linear_filter() �����Լ��� ������ �� �ִ�.
		***	3.	�ʿ信 ���� linear, nonlinear ������ ��� ����ϴ� filter�� ������ �� �ִ�.
		***	������,
		***	��Ī�� �����ϰ� �� �˷��� filter, Ȥ�� filter �з��� �̸��� ������ ������ ���� �Ǵ� ����, ���ӻ��� �˰������� ������,
		***	�̵��� customizing�� filter�� ���� �̸����� �����Ͽ� ����ؼ� �� �ȴٴ� ���� ����ؾ� �Ѵ�.



		Convolution (�������, �ռ���)  :  �߽� ȭ���� ���� �߽��� ������ ���� ȭ�Ұ����� ���� ��(weighted sum)���� ��ü�ϴ� �����̴�.
		Correlation (�ڸ����̼�, ���)		kernel�� ����ġ(weight)�� ���ȴ�.

											mask ����ġ�� ��(kernel ������ ����)��, ��� ������ ��ü���� ��⿡ ������ �ش�.
											���� 1�� ��, ������ ��Ⱑ �����ȴ�.
											// edge-detection(������ ����)������ 0�̶� ���� �ȴ�.

											// linear filter�� ���Ǵ� �������� correlation(�ڸ����̼�)�� convolution(�������, �ռ���)�� �ִ�.
											// correlation : SOP(sum of products)�� �����ϴ�.
											// convolution : origin Ȥ�� kernel�� 180�� ȸ���Ͽ� correlation�� �����Ѵ�.  // �� 180�� ȸ���ϴ����� ���� ������ ��ó���ǿ� �ִ�.

											// correlation, convolution ��θ� SOP��� �ǹ̷� ��Ʋ� convolution�̶� �θ��⵵ �Ѵ�.

											// �� �ۿ��� linear filter�� ������ ���Ǵ� ������ correlation���� �����Ͽ���.
											// �����ϱ� ���ؼ� ������ �ϵ�, ���� ��ü�� convolution���� �����Ͽ���.

											***	��������.									+0  +0  +0
												�Է°� ����� ������ filter�� kernel[3*3]  =	+0  +1  +0
																							+0  +0  +0







*** convolution������ ����ϴ� linear filter ������ �־ �߿��� �� ����.

			LPF���� ��Ȯ�� �����ϰ� ��Ȯ�� kernel�� �����־��ٸ�, ����� overflow�� �Ͼ�� �ʴ´�.
			����Ȯ�� kerenl�� �����־����� Ȯ���ϱ� ���ؼ���, overflow�� �ذ����� �ʴ� ���� debugging�� �� ���� �Ѵ�.
			�ֳ����, ���ڿ������� ������ ��µǾ� ������ Ȯ���ϱⰡ ���� �����̴�.

			sharpening������ overflow�� �Ͼ�� ���� �翬�ϸ�, �翬�� �̸� �ذ��ؾ� �Ѵ�.
			���⼭ ����� normalizing�� �ؼ��� �� �Ǹ�, overflow�� ��� 0 �Ǵ� 255�� ������ �����ؾ� �Ѵ�.

			���÷�, ����ġ�� ��(1)�� sharpening������ ������ ��⸦ �����ϸ� ������ sharp�ϰ� �Ѵ�.
			�׷��� overflow�� �ذ��ϱ� ���ؼ� normalizing�� ��ģ�ٸ�,
			�Ȱ��� ��µǾ�� �ϴ� ���������� ��⿡ ������ ��ģ��.(��ǻ� ��� ȭ�Ұ��� ��Ʋ����.)



			HPF������ overflow�� �Ͼ�� ���� �翬�ϸ�, �翬�� �̸� �ذ��ؾ� �Ѵ�.
			�ذ� �����, convolution���꿡 ���� mask ����ġ�� ��(kernel ������ ����)�� 0�̳�, 0�� �ƴϳķ� ������.

					1.	����ġ�� ���� 1�� ��.
							convolution �Ŀ� ����� normalizing�� �ؼ��� �ȵǸ�, overflow�� ��� 0 �Ǵ� 255�� ������ �����ؾ� �Ѵ�.
							
							���÷�, ����ġ�� ���� 1�� sharpening������ ������ ��⸦ �����ϸ� ������ sharp�ϰ� �Ѵ�.
							�׷��� overflow�� �ذ��ϱ� ���ؼ� convolution �Ŀ� normalization�� ��ģ�ٸ�,
							�Ȱ��� ��µǾ�� �ϴ� ���������� ��⿡ ������ ��ģ��. (��ǻ� ��� ȭ�Ұ��� ��Ʋ����.)
							�̸� �ذ��ϱ� ���ؼ� sharpening�� ������ ������ �����ϵ��� �����Ѵٸ�, �̴� �� �̻� sharpening�� �ƴϰ� �ȴ�.

					2.	����ġ�� ���� 0�� ��.
							������ ����/�����ĸ� ���� �Ǹ�, �� ����ȭ���� ���� �����ĸ� ��� ����� �������� �����ϰ��� �ϴ� filter�� ���� �ٸ���.

							embossing�� ��ü���� ������ �Ʒ��� ����.
							�����ĸ� �����Ѵ�.  ->  [-128, 127] ������ ����ȭ�Ѵ�.  ->  ���Ⱚ���� +128�� ���Ѵ�.
							��/���� ���� �Ŀ� ��/�� ������ �ϵ���, ���Ⱚ ������ ���� ��������(�Լ� ���ο��� ��ȯ ������) �Ѵ�.
							embossing���� normalization�� �̷��� ���ȴ�.




***	sharpening�� unsharp-masking/edge-detection�� ��������, �ٷ� ���� ���� �����ߴٸ� �� �� �ִ�.
	��� ������ ������ ������ �� �ִ���, �������� �����̴�.

	sharpening��, ����ġ�� ���� 1�� kernel�� ���� �̻�, ������ ��⸦ �����ϱ� ���ؼ� ����� normalizing�� �ؼ��� �ȵǹǷ�,
	��¿ �� ���� overflow�� �Ͼ ��ŭ ���� ������ ������ �Ѵ�.
	���� ������ ��ư� �ؼ� �츱 ���� ������, �׷� �ٿ� ���� ��Ȯ�� unsharp-masking Ȥ�� edge-detection�� ����ϴ� �� �ξ� ������.

	unsharp masking��, LPF�� ��ġ�Ƿ� overflow�� �Ͼ�� �ʾƼ� ��� ������ ������ ������ �� �ִ�.
	edge-detection��, �����ĸ� ������ ���� ��ó�� filter���� normalizing ����� �ִ�.
			





	< LPF > (Low-Pass Filter, ������ ����)  :  Smoothing(������)  ->  Blurring(������), Noise Reduction(���� ����)

		LPF �� linear filter������ mask�� ����Ǵ� kernel�� ��� ����(mask�� ��� ���)�� ����̴�.


			�� Mean Filter (��հ� ����)
				Linear Spatial Filtering(���� ���� ���͸�)�̸�, mask�� �����ϰ� ����ġ�� ����ȴ�.
											1  1  1
				kernel_mean[3*3]  =	 1/9 *	1  1  1
											1  1  1



			�� Gaussian Filter (����þ� ����)
				// ���� : https://sungw.tistory.com/24

				Linear Spatial Filtering(���� ���� ���͸�)�̸�, mask�� �߾��ϼ��� ���� ����ġ�� ����ȴ�.

				����þ� ����(Gaussian distribution) �Լ��� �ٻ��Ͽ� ������ ���� ����ũ�� ����ϴ� ���͸��̴�.
				����þ� ���� Ȥ�� ���� ����(normal distibution)��, ����� �߽����� �¿� ��Ī�� �� ����� ���� Ȯ�� ������ ���Ѵ�.

				// �ȼ� ���� �ް��ϰ� ����Ǵ� ���� �ٹ濡 ������ ����þ� ���Ͱ� ����Ǹ� �����Ӹ� �ƴ϶� ���� ���б��� �Բ� �����ϰ� �ȴ�.
				// ��, ������ �پ��鼭 �Բ� ������ �������� ������ ��ü�� ������ �帴�ϰ� �ٲ��.
				// �̷��� ������ �����ϱ� ���� ���� ������� ���� ������ �״�� �����ϸ鼭 ������ �����ϴ� ���� ���� ���� ���� ����(edge-preserving noise removal filter)�� ���� �����߰�,
				// �̷� ���� bilateral filter(����� ����)�� ���ܳ�����, ����� ���ʹ� ���� ������ ���� �� �����ϸ鼭 ����þ� ������ ȿ�������� �����ϴ� �˰������̴�.



			�� Bilateral Filter (����� ����)

				Nonlinear Spatial Filtering(���� ���� ���͸�)�̴�.

				// gaussian noise(����þ� ����) ���ſ� Ź���ϴ�.



			�� Order-Statistic Filter (����-��� ����)
				// ���� : https://m.blog.naver.com/PostView.naver?blogId=kss9409&logNo=221525830164&navType=by
				// ���� : https://everyday-image-processing.tistory.com/170

				Nonlinear Spatial Filtering(���� ���� ���͸�)�̴�.
				��, mask�� kernel�� ��ġ�� ������, kernel�� ���� weight(����ġ)�� ���� convolution������ ����.
				���, �߰��� ������ ���� mask�� ������ �ʿ��ϴ�.

				���ĵ� mask���� ��� �˰������� �����ϴ��Ŀ� ���� ���� ������ filter�� ������.


					�� Median Filter (�߰��� ����)  :  Noise Removal(���� ����)

						���ĵ� mask���� �߰����� �����Ѵ�.

						salt-and-pepper noise(������/�ұ�&��������) ���ſ� Ź���ϴ�.

						mask�� ���°� �پ��ϸ�, ���̾Ƹ������ ���� ���� ���̰�, �������� ���� ȿ���� ����.

						median filter�� mask���¿� run-time ������ ���� ������ �˰�������� �����Ѵ�.

							�� hybrid median filter : median filter�� ����, �ڳ��� ��Ư������ ������ �˰������̴�.
							�� separable median filter : ??
							��



					�� Max Filter (�ִ� ����)

						���ĵ� mask���� �ִ��� �����Ѵ�.
						// ���� : mask�� ���� ���� ���׿����ڷ� �����ϴ�.



					�� Min Filter (�ּڰ� ����)

						���ĵ� mask���� �ּڰ��� �����Ѵ�.
						// ���� : mask�� ���� ���� ���׿����ڷ� �����ϴ�.



					�� Mid-point Filter (�߰��� ����)

						���ĵ� mask���� �ִ񰪰� �ּڰ��� ����� �����Ѵ�.
						// ���� : mask�� ���� ���� ���׿����ڷ� �����ϴ�.



					�� ��-trimmed Mean Filter (��-���� ��հ� ����)
						// ���� : https://m.blog.naver.com/dic1224/80182190750
						// ���� : https://m.blog.naver.com/PostView.naver?isHttpsRedirect=true&blogId=dic1224&logNo=80180971021


						mean filter�� median filter�� ������ ���� �˰������̴�.
						��(MIN) == mean filter
						��(MAX) == median filter

						�ῡ ���� �� : { 0.1, 0.2, 0.3, 0.4, 0.5 }

						�Ǽ� �� ���� ���� ���ĵ� mask�� ���ʿ��� ������ ������ ������.


							// n : mask ��� ������ ����. ( mask[3*3]���� n = 9 )
								int TEMP = (int)(n * ��);
								for (int i = TEMP; i < n - TEMP; ++i)
								{
									int SUM += mask[i];
								}
								return SUM / (n - 2 * TEMP);

								N_remainder = n - 2 * TEMP;

								output[width * y + x] = SUM / N_remainder;


							// n = 9(3by3)���� �� ���� ���� TEMP����, ��հ��� ���ϱ� ���� ���� ������ ����.
								��(0.0)  ->  TEMP(0), N_remainder(9)		XXXXXXXXXXXX
								��(0.1)  ->  TEMP(0), N_remainder(9)  == mean filter
								��(0.2)  ->  TEMP(1), N_remainder(7)
								��(0.3)  ->  TEMP(2), N_remainder(5)
								��(0.4)  ->  TEMP(3), N_remainder(3)
								��(0.5)  ->  TEMP(4), N_remainder(1)  == median filter
								��(0.6)  ->  TEMP(5), N_remainder(-1)?	XXXXXXXXXXXX







	< HPF > (High-Pass Filter, ������ ����),    < Edge Filter > (edge detection/Enhancement filter) (������ ����/��ȭ ����)
		// ���� : https://m.blog.naver.com/PostView.naver?blogId=kss9409&logNo=221538477716&navType=by


			�� Sharpening (������, ÷��ȭ)

				������ ��⸦ �����԰� ���ÿ� edge�� �� �����ϴ� ����̴�. (����ġ�� �� = 1)

				noise ���� edge�� �� �� ������, �̸� �����ϱ� ���ؼ� �Ϲ������� smoothing(noise-reduction)�� ��ó�� �������� ����Ѵ�.
				�̷��� ��ó�� ������ unsharpening, ��� ������ ���� unsharpened image�� �� �� �ִ�.

				// �Ϲ����� HPF�� ���� ������ ������ ��⿡ �ش��ϴ� �κп� �������� �̵��� �����ν�
				// HPF���� �߻��ϴ� ���� ���� ���ļ��� ������ �սǵǴ� �������� �������ִ� ����ũ�̴�.
				// sharpened������ origin�� origin_HPF(edge)�� ���� �Ͱ� ����� ����� ��´�.

				sharpening�� �ڱ� �ڽ��� �������� �ϹǷ�, ����ũ�� �ֺ��� ����� ���ǵ��� �����.
				���� ������ ȭ�� ���� n��, �ֺ��� ȭ�� ���� -1�� �Ͽ� ���� 1�� �ǵ��� ����ũ�� �����.
				kernel�� �߾Ӱ��� Ŭ���� ���� ���󺸴� sharpening ȿ���� ũ��.
											+0  -1  +0			+1  -2  +1			-1  -1  -1			+0  -2  +0
				kernel_sharpening[3*3]  =	-1  +5  -1	  or	-2  +5  -2	  or	-1  +9  -1	  or	-2  +9  -2	  or	...
											+0  -1  +0			+1  -2  +1			-1  -1  -1			+0  -2  +0



			�� �Ʒ��� ���� ������ �����ϰ� ���� �� �ִ�.
				edge = (origin) - (origin_smoothing)		�� [��]
				or
				edge = (origin_sharpening) - (origin)		�� [��]


					�� Unsharp Masking (����� ����ŷ), Highboost Filtering (���̺ν�Ʈ ���͸�)
						// ���� : https://everyday-image-processing.tistory.com/141

						[��]�� ����� ���Ǹ�, HPF�� �̺��� ���� ������ ������� �ʰ� edge�� ã�ų� ������ �� �ִٴ� ������ �ִ�.


						-- �Ʒ��� ���� ������ edge�� ���Ѵ�.
						(edge) = (origin) - (origin_smoothing)

						-- ���� ���� edge�� �󸶳� ���� �������� ���� ��� k(k >= 0)�� �����Ͽ� ���� ����,
						(origin_sharpening) = (origin) + ( k * (edge) )

						-- �� ������ ���� ������ ����� unsharp mask(����� ����ũ)�� �ϸ�,
						-- (k = 1)�̸� unsharp masking�̰�,
						-- (k > 1)�̸� highboost filtering�̴�.



			�� Isotropic Filter (��漺 ����)

					�� Laplacian Filter (���ö�þ� ����)
						// ���� : https://everyday-image-processing.tistory.com/140

						laplacian�� ���� ������ kernel�� ����ϴ� filter�̴�.
						// ���п��� ���ö� ������(Laplace operator)/���ö�þ�(Laplacian)�� 2�� �̺� �������� ��������, ������ �߻��̴�.
													+0  +1  +0			+1  +1  +1			+0  -1  +0			-1  -1  -1			-1  +2  -1
						kernel_laplacian[3*3]  =	+1  -4  +1	  or	+1  -8  +1	  or	-1  +4  -1	  or	-1  +8  -1	  or	+2  -4  +2	  or	...
													+0  +1  +0			+1  +1  +1			+0  -1  +0			-1  -1  -1			-1  +2  -1

						laplacian filter�� ��ģ ������ g(x,y)�� �ϸ�,

						g(x,y) = f(x,y) + c[��f(x,y)]    ( c = +1 or -1 )

						������ ������ �����ϰ� kernel�� ������� (a), (b), (c), (d)�� �� ��,
						kernel�� �߾Ӱ��� ������ (a), (b)�� ���ؼ��� [ c = 1 ] �̸�,
						kernel�� �߾Ӱ��� ����� (c), (d)�� ���ؼ��� [ c = -1 ] �̴�.



			�� Anisotropic Filter (���漺 ����)

					�� Embossing (������)
						// ���� : https://psh7286.tistory.com/entry/OpenCV-4-7-%ED%95%84%ED%84%B0%EB%A7%81

						������ ��� pixel���� ���̷� �ϴ� 3D��������,
						{
							correlation : kernel�� [-1 -> +1] �������� �޺��� �ٴ�.
							convolution : kernel�� [+1 -> -1] �������� �޺��� �ٴ�.
						}

						embossing�� ��ü���� ������ ������ ����.
						�����ĸ� �����Ѵ�.  ->  [-128, 127] ������ ����ȭ�Ѵ�.  ->  ���Ⱚ���� +128�� ���Ѵ�.

						// �ᱹ�� ���� ���� ��°����İ� 255�� ���� ������, ���� ���� �ϰ������İ� 0�� ���� ������ �ȴ�.

													-1  -1  +0			-1  +0  +0			+0  +1  +0
						kernel_embossing[3*3]  =	-1  +0  +1	  or	+0  +0  +0	  or	+0  +0  +0	  or	...
													+0  +1  +1			+0  +0  +1			+0  -1  +0

						// ���� ���� ���� ������ �� ������, ���� ������ �ٸ��� �Ͽ� 16������ �ִ�.

						// ������ ��輱 ������ ���� ����� ��ġ�Ҽ���, ��輱�� ������� �ʴ´�.
						// ���� ���⿡ ���� kernel�� ���µ� �پ��ϴ�.

						// kernel�� ����, �޺��� ���� ���⿡�� �ؾ� ������ ������ �ռ��Ͽ� �ٹ���� ������ �����ϰų�,
						// ȸ�������� �������� �ʰ� edge-detection���� ����ϴ� ����, ��õ���� �ʴ´�.
						// ������ filter�� ������ ������ �����ϸ�,
						// embossing filter��, "������"�̶�� �������� �����ִ� filter�� �� �����μ� �����Ǿ�� �Ѵ�.
						// ���� ���� ������ filter�� customized filter�� ���� �з��ؾ� �Ѵ�.



					�� Roberts cross-gradient operator (�ι��� ����-�׷����Ʈ ������), Sobel operator (�Һ� ������)
						// ���� : https://everyday-image-processing.tistory.com/141

						gradient�� ���� ������ kernel�� ����ϴ� filter�̴�.
						// ���п��� �׷����Ʈ��, ��� �ٺ���/�ٹ�� �Լ��� ���� 1�� �̺��� ���Ѵ�.

						[ �����ۿ� �ִ� ���ĵ� ]

						���� �� ���� kernel�� ������ ����� ���� ���Ŀ� �����ϴ� ������ edge_detection�� ����ȴ�.

						// ������ �𸣱� ������ kernel�� �˾Ƶ� �ǹ̰� ����. kernel�� �ñ��ϸ� �˻��ϸ� ���´�.







	< Histogram Modification > (������׷� ����)  ==  Histogram Matching (������׷� ��Ī)  ==  Histogram Specification (������׷� ����ȭ)

		�Է� ������ histogram�� ���ϴ� histogram���� ��ȯ�ϴ� ����̴�.
		// ������ brightness�� contrast�� ������ �� �ִ�.

		���� ��ü���� �����ϴ� global processing��, (���� ó��)
		���� �Ϻκп��� �����ϴ� local processing�� �ִ�. (���� ó��)

			// ���� ó�� ����� ������ �������� ��󿡴� �����ϳ�, ���� ���� ���� ������ ������Ű�� ���Ѵٴ� ������ �ִ�.
			// �̸� ���ؼ� ������׷� ��Ȱȭ�� ������׷� ������ ��ü ���� �����ϴ� ���� �ƴ϶�,
			// �߾� ȭ�Ҹ� �������� �ϴ� ���󺸴� ���� window���� ó���ϰ� �� �ȼ� �̵� �� �ش� window���� ó���ϴ� ����� ���� ó�� ����� �ִ�.

			// �̶� window size�� 3*3, 5*5�� ���� ���� ũ��� �ϸ�,
			// �پ��� window size �߿��� ��� ������ ǰ���� ���� ���� window�� �����Ѵ�.

			// ���� ó�� ����� ������׷� ������, ���� ���͸��� ���� ������ �ٸ� �� ���� �����ϴ�.


					continuous variable(���� ����), discrete variable(�̻� ����)

					probability function(Ȯ�� �Լ�)
						Ȯ�� ������ ���� �� �ִ� ����� Ȯ���� �Ҵ��ϴ� �Լ��� �ǹ��ϴ� �Ϲ����� ����̴�.
						continuous/discrete variable ��� �����Ѵ�.

					probability distribution(Ȯ�� ����)
						Ȯ�� ������ ���� �� �ִ� ���鿡 ���� Ȯ���� ��� �����Ǿ� �ִ����� �����ϴ� ���� �����̴�.
						Ȯ�� ������ ���ۿ� ���� ������ ��������, ��� ������ ����� �׿� ���� Ȯ���� �����Ѵ�.
						continuous/discrete variable ��� �����Ѵ�.

					CRV (Continuous Random Variable, ����Ȯ������)
					PDF (Probability Density Function, Ȯ���е��Լ�)
					CDF (Cumulative Distribution Function, ���������Լ�) : PDF�� ������ �����̴�.

						PDF(probability density function, Ȯ���е��Լ�)�� PDF(probability distribution function, Ȯ�������Լ�)�� ���̿� ���ؼ�
						-> https://velog.io/@groovallstar/%ED%99%95%EB%A5%A0-%EB%B6%84%ED%8F%AC-%ED%95%A8%EC%88%98%EC%99%80-%ED%99%95%EB%A5%A0-%EB%B0%80%EB%8F%84-%ED%95%A8%EC%88%98%EC%9D%98-%EC%9D%98%EB%AF%B8

					DRV (Discrete Randon Variable, �̻�Ȯ������)
					PMF (Probability Mass Function, Ȯ�������Լ�)
					CMF (Cumulative Mass Function, ���������Լ�) : PMF�� ����/���� �����̴�.


		LUT (Loop-Up Table, ������̺�) : histogram ������ ���� Ư�� LUT�� �����ϰ�, ������ LUT�� ���ļ� ������ ������ ��´�.

		// ���� �������� CRV�� ������ PDF, CDF�� ������ �� ������,
		// ��������� digital image�� histogram�� DRV�� ������, histogram�� LUT�� ���� PMF, CMF�� �ǹ��Ѵ�.



			�� Histogram Scaling (Stretching)

				�ش� ������ ������ �ø���.



			�� Histogram Scaling (Shrinking)

				�ش� ������ ������ ���δ�.



			�� Histogram Sliding

				������ shift�Ѵ�.



			�� Histogram Matching (������׷� ����)
				// ���� : https://everyday-image-processing.tistory.com/134

					// �ϳ��� ���ذ� �ȵȴ�.



			�� Histogram Equalization(����ȭ, �յ�ȭ, ��Ȱȭ, spreading)

				���� ����� �����ϰ� ������Ŵ���ν� �ȼ� ���� ������ ������ �Ѵ�.
				�̻������� PMF�� �����ϰ�(flatter), CMF�� ������ �������� ������ �ȴ�.
				�Ϲ������� ������ CMF�� LUT�� ����ϴ� ����� �ִ�. ( -> �������� ���ֽ� histogram equalization�� ù ������ �̰Ŵ�.)

				// ���ۿ� �ذ����� ���� ���� : https://scienceon.kisti.re.kr/commons/util/originalView.do?cn=JAKO201015233599145&oCn=JAKO201015233599145&dbt=JAKO&journal=NJOU00294921



					�� equalization�� �ַ� ���Ǵ� LUT(Look-Up Table)�� ���Ͽ�.

							������, ������ LUT�� ���ļ� equalized������ ��´�.
							LUT�� ��� ������� �������, ��� ���������� ���� ������ equalizing ����� �ٸ���.

							grayscale�� ������ LUT�� ���¸� ���ϴ� ������ �����غ���.

							input[j][i] -> x		// �Է� ������ Ư�� ȭ�Ұ�.
							output[j][i] -> y		// ��� ������ Ư�� ȭ�Ұ�.
							LUT -> (function) F

							������ LUT�� ���ļ� EQ������ �����Ǵ� ���� �������� ��Ÿ����,
							"y = F(x)" �̴�.

									e.g.	������ LUT�� ���ļ� �����ϰ� ���� �� LUT�� �׷����� "y = x" �̴�.

							�Լ�(LUT)�� input(x��)�� output(y��)�� �־�� �Ѵ�.
							1���� �迭�� ���,
							������ index ��ȣ�� x���̰� �ش� index��ȣ�� ���� y���̹Ƿ�,
							���� ���ؼ� 1���� �迭�� LUT�� ����� �� �ִ�.

							�����Ͽ� �ڵ�� ��Ÿ����,

							"output[j][i] = LUT[input[j][i]];"


							"10����_����_2"������ LUT�� �̸� ����� �� LUT�� �ƴ�, input image�� ���� ������ LUT����.
							���� ���� LUT�׷����� ����� �дٸ� �� ��ü�� �������������� ���ͱ���� ���̴�.







	< image binarization > (���� ����ȭ + thresholding(�Ӱ谪 ����))  ==  image segmentation (���� ����)

		����ó������ ����ȭ��, � �־��� �Ӱ谪(threshold, T)���� ���� �ȼ����� ��� MAX��, �׷��� ���� �ȼ����� ��� MIN���� �ٲٴ� ���� ���Ѵ�.

		// ���߰���� ��ü�� ǥ�������� ����ȭ ������ ���� [MAX, MIN]���� �ٸ��� �Ѵ�.
		// ���͸� ����� ���� ���ϰ�迡���� [MAX, MIN]���� �ϳ��� ������.
		// �Ϲ������� ����ȭ�� ������ monochrome���� ǥ���ϸ�, �츮�� �ǽ����� ����ϴ� ����� = grayscale�� monochrome�� ���Ѵ�.

		// thresholding = �Ӱ谪 ����, �Ӱ�ó��, �Ӱ谪 ó��

		�Ӱ谪�� ���ϱ� ���ؼ�, ������ histogram�� Ȱ���� �� �ִ�.

		"���� ���� ����ȭ", "���� ���� ����ȭ"�� ������.







	< Global Fixed Thresholding > (���� ���� ����ȭ)

		�ϳ��� �Ӱ谪���� ����Ͽ� ����ȭ�ϴ� ����̴�.
		�� �Ӱ谪�� ��� �ڵ����� �����ϴ��İ� �߿��ϴ�.

		optimal threshold (optimal T) : ����ȭ�� ���� �з� ����(classification error)�� �ּ�ȭ�ϴ� �Ӱ谪�̴�.

			�� Lab�� flower����  or  �� ������ ���� �۾�
			���� ������� histogram�� ���� �Ϻ��ϰ� ��� ������ ��ü ������ �� Ŭ������ �����Ƿ�,
			�Ӱ谪 T�� ���ϴ� ���� ���� ����.

			�� Otsu's method (������ �˰�����) : ������ �Ӱ谪�� ã�� ������ �˰������̴�.
			�Ӱ谪 T�� �������� ���� �ȼ����� �� Ŭ������ �з����� ��,
			�� Ŭ�������� intra-class variance�� �ּ�ȭ�ϰų� �Ǵ� inter-class variance�� �ִ�ȭ�ϴ� T�� ã�� ����ȭ ����̴�.

			�� google search -> "adaptive thresholding" -> image.
			�� Lab�� lena����.
			������ ���󼭴� ���� ������ ���� �ϳ��� �Ӱ谪(threshold)�����δ� � ���� �־ ��Ȯ�� ����ȭ�� ���� ��찡 �ִ�.







	< Local Adaptive Thresholding > (���� ���� ����ȭ)
		// ���� : https://darkpgmr.tistory.com/115
		// ���� : https://sungw.tistory.com/25?category=926149

		�ȼ� ��ġ���� ���� �ٸ� �Ӱ谪�� ����ϴ� ���� thresholding ����� ���� ����ȭ ����̴�.
		Pixel Area(Group) Processing(ȭ�� ���� ó��, �ȼ� �׷� ó��)�� ���Ѵ�.
*/

/*
�ѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤ�
�� ��ó��, �̺з�
�ѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤ�

	1.
	�����̶� ���� �ߴ�, ��ȭ���� ������ ���� �̿��� ����ȭ.
	2.
	1�� ������� �������� �ƴ� ���� ��(%).

	>> 1��, 2�� ��� mask�� 2D �̻�̺б׷����� Ȱ���� "���� ���� ����ȭ"�̴�.





	openCV : �ǽð� ��ǻ�� ������ �������� �� ���α׷��� ���̺귯���̴�. C/C++�� �ִ��� �𸣰ڴµ�, �����ϴϱ� �˾Ƶ���.





	���۸� ���, ��κ��� ���� �������� �������� ���Ͻ��Ѵ�.
	������ openCV������
	���� �������� �������� ���̸� ã�µ� ��ǻ� ����������, �� �� ���� ã�Ҵµ� �̸� ���⿡ ������.

		���� : https://marisara.tistory.com/entry/%ED%8C%8C%EC%9D%B4%EC%8D%AC-openCV-13-%EB%B8%94%EB%9F%AC%EB%A7%81blurring%EC%8A%A4%EB%AC%B4%EB%94%A9smoothing%EC%83%A4%ED%94%84%EB%8B%9Dsharpening

				�� Smoothing (������)
				�������� �������� ����ϰ� �ֺ� ȭ�ҵ��� ������� ����������,
				�ֺ� ȭ�ҵ鿡 ������ �־� ����Ѵ�.


				e.g. kernel_smoothing[3*3]			1  2  1
											1/16 *	2  4  2
													1  2  1

					// �������ڸ�,
					// blurring : ������ ����ġ�� mask, ������ ����� kernel�� ����� filter�� ���Ѵ�.
					// smoothing : blurring �̿ܿ� ����ġ/����� �ٸ� �͵��� ���Ѵ�.

					// �̿� ������,
					// ��հ� ���ʹ� mean-blurring�� �ǰ�, ����þ� ���ʹ� Gaussian-smoothing�� �ȴ�.
					// �׷��� �� openCV���� ����þ� ������ �̸��� GaussianBlur()�̴�.??

					// ���� �� filter�� "-blurring" �Ǵ� "-smoothing"���� ���۸��� ���� ���� �� ���,
					// �ѱ�, ����� ��� ������ ���� �����Ƿ� �� ���� ���Ͻ��Ѵ�.



		���� : https://webnautes.tistory.com/1044
		���� : https://www.geeksforgeeks.org/spatial-filtering-and-its-types/

				smoothing : ������ �帮��(blurring) �ϰų�, ���� �����ϴ� ����� ����(noise-reduction)�ϱ� ���� ���ȴٰ� �Ѵ�.

					// ���⼭�� smoothing�� blurring�� noise-reduction�� �����Ѵ�.



	��� : ����Ʈ�� 3���� �з��غ���.

		1. ó�� ����Ʈ.

		2. �� ��° ����Ʈ.

		3. �� ������ �� smoothing�� blurring�� ���ٰ� �Ѵ�.
			�̷��� ��� ���Ǹ� �ߴ� �� �ߴ� ������ ������
			smoothing Ȥ�� blurring �� �ϳ��� ������ �Ǿ� �ִ�.


		1���� �� �ϳ� ���̰�, 3���� 90% �̻�?
		2���� ������ ���� �ʴ�.
		���� 2������ ����, �����ϰų� ���������� ����.





	���� : https://sijoo.tistory.com/40

		noise model (���� ��) : ������ �����Ǵ� ��Ŀ� ���� �з��ȴ�.

			�� gaussian noise(����þ� ����) : ���� ������ ���� noise�̸�, ��ǥ���� �����̴�.
			�� Rayleigh(����)
			�� Erlang/Gamma(����)@@@@@
			�� Exponential(����)
			�� Uniform(����)
			�� Impulse(���޽�) : salt-and-pepper noise(�ұ�&���� ����, �� ����)





	���� : https://eehoeskrap.tistory.com/99

		������ ����  ->  ���� ���޽� ���� ����, ���� ���޽� ���� ����.





	�� �ۿ����� mask, kernel, window, template �� ����� ������ Ȯ���ϰ� �и��Ͽ� ����ߴ�.





	median filter�� nonlinear-filter�μ� noise�� �Ϻ� ���Ű� �����ϴ�.
	linear-filter�δ� noise�� �Ϻ� ���Ű� �����. �츮 ���� �Ⱥ��̸� ���̱� ������.
	�׸��� noise reduction�� blurring�� ���ÿ� �۾��ϱ� ���� median filter��, �ٸ� LPF��� ���Ͽ� ����� ����.
	�������� ������ �����̰�, ���� ����� ������ ���� �۰�, ��ư ���� ����.
	������ median filter�� ����/���ۿ��� ���� �״�, �̸� ã�ƺ���.





	���� : https://velog.io/@lighthouse97/%EC%98%81%EC%83%81Image%EC%9D%98-%ED%95%A9%EC%84%B1%EA%B3%B1Convolution%EC%9D%98-%EC%9D%B4%ED%95%B4

		CNN(convolution neural network)������ convolution������ ������ �ڼ��� �����Ѵ�. // AI�о߿� ������.
		���� ���� ���ؿ� ��� ������ ���/������ �� �� �ִ�.

			Stride(��Ʈ���̵�, ����)
				���͸��� ���͸� �̹����� ���� �����̸鼭 �Ѵ�.
				�̶� ���Ͱ� �ѹ��� �����̴� �Ÿ��� ����, ���� ��Ʈ���̵�(stride)��� �θ���.
				Ŀ���� �� ĭ�� �����̸� ��Ʈ���̵� ���� 1�� �ǰ�, �� ĭ�� �����̸� 2�� �ȴ�.

			feature map(Ư�� ����)

			shared weight(����ġ ����)

			padding(�е�)

			activation function(Ȱ�� �Լ�)

			ReLU(rectified linear unit)

			pooling(Ǯ��) - max-pooling, average-pooling, stochastic-pooling

			overfitting(��������)

			channel(ä��)

			recognition(�ν�)





	���� : https://m.blog.naver.com/PostView.naver?blogId=kss9409&logNo=221515037321&navType=by

		Ends-in Ž�� ���
		threshold ���� ��� 3����





	���� : https://m.blog.naver.com/PostView.naver?blogId=kss9409&logNo=221525830164&navType=by

		spatial processing(���� ó��)  ==  neighborhood processing

		average filter(��� ����)  ->  box filter(���� ����, ������ ����ġ), weighted average filter(����ġ ��� ����)

			box filter(���� ����) : mena filter(��հ� ����)
			weighted average filter(����ġ ��� ����) : gaussian filter(����þ� ����)

		���� Ȯ�� �е� �Լ��� ����
			1. ���콺 ����(Gaussian Noise) : ���� �߻��ϴ� ������ ��κ� ���콺 ������ ������.
			2. ���� ����(Uniform Noise)
			3. ���޽� ����(Impulse Noise)





	������ ���� ó���� "���� ������(problem-oriented)"�̱� ������, Ư�� ������ Ǯ�� ���ؼ� �پ��� ����� ������ �� �ִ�.





	���� : https://everyday-image-processing.tistory.com/137

		linear filter�� ���Ǵ� �������� spatial correlation(���� �ڸ����̼�)��, spatial convolution(�ռ���)�� �ִ�.
		�� ���� SOP �����̶�� ���� ����������,
		convolution��, correlation�� �����ϱ� ���� mask�� 180�� ȸ���Ѵ�.

		// �׷��� correlation ��ü�δ� SOP(sum of products)�� �ǹ̰� ���� ������ �ʹ�.

		// �׷���, linear filter�� ���ǿ� ���� convolution�� ȸ���� �ǹ̰� ���� �� �ƴѰ�??? ( + �ǹ� ���� ���� �´�. )
		// ������ �̼��� �����Ե� correlation�� ���� ����� ��������, convolution�̶� ������� ȸ�� ���� ����� ����.
		// �ϴ��� convolution�̶� ���� ����, ���߿� filter ������ �� ã�ƺ���.





	smoothing spatial filter(������ ���� ����)  ->  smoothing linear filter(������ ���� ����), order-statistic filter(����-��� ����)





	2���� ��ĳ����� ���꿡���� ����for���� �ʿ��ѵ�,
	�̸� �� ���� ó���ϵ��� vector�� ��ȯ�Ͽ� ������ �� �ִ�.





	���� : https://everyday-image-processing.tistory.com/?page=35

		���ļ� ���� ���͸��� ���⼭ �̾ �ϸ� �ȴ�.
		���� ���� ������ �ʿ�� �ϱ� ������, ���߿� ������Ʈ�� �ʿ��ϸ� �����.





	���� : https://www.mathworks.com/help/images/what-is-image-filtering-in-the-spatial-domain_ko_KR.html

		���� ���͸��̶� ��� �ȼ��� ���� �Է� �ȼ��� �̿� �ȼ����� ���� ���� ������ �Ǵ� ���͸��Դϴ�.




�ѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤ�
�� convolution�� 180�� ȸ�� ������ ���ؼ� ã�ƺ���.
�ѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤ�


	���� : https://wiserloner.tistory.com/845
	"���� ó�� �о߿����� ��κ� ���Ͱ� ��Ī������ �������̱⿡, ���͸� ������ ������� �����̶�� �θ��� �ֽ��ϴ�."


		�̰�, convolution ������ Ȯ���ϰ� �������.
		���� convolution�̶� �ϵ�, �ϴ� ������ correlation���� �Ѵٰ�.
		embossing���� �� �� ���� ������ ������ �����ص���.

			[�ڸ����̼�]
			- ���͸� ���� ����� �˰���?
			���� �� ���ҵ�� ������ ���� �ȼ� ���� ������ ��, �� ���� ��� �����ִ� �����̶�,
			�����ڸ� �� ���� ��ȣ���� '���缺'�� �Ǵ��ϴ� �����̶� �Ҽ� �ֽ��ϴ�.
			�̷� ���� ��ȣ ó���о߿����� Correlation �Ǵ� ����̶�� �մϴ�.

			[�������]
			- ���͸� ������ �ڸ����̼��̶���� �ϴµ�,
			�ٸ� ���δ� ��������̶���� �մϴ�.
			- �������Convolution�� �ռ�������,
			�ڸ����̼ǰ� ������ �����ѵ�, ���ݴ� ������ ���ڸ�,
			���Ͱ� �߽����� �������� ��Ī�ǰ� �̷���� ���͸� ����ϴ� ���Դϴ�.
			- ���� ó�� �о߿����� ��κ� ���Ͱ� ��Ī������ �������̱⿡, ���͸� ������ ������� �����̶�� �θ��� �ֽ��ϴ�.


		�̰�, "180�� ��Ģ"������?? https://m.blog.naver.com/blockbuster3/220597830367
		180�� ��Ģ�̶� ���� ��Ģ�� �����̴�.
		�׷��� �� ��Ȯ�� ���ϸ� �������� ���Ѿ� �ϴ� ��Ģ�̶�� ���ٴ� ���ùٸ� ������ ���� �Կ��ÿ� ���Ѿ� �ϴ¡� ��Ģ1�̴�.


	���� : http://contents.kocw.or.kr/document/lec/2012/Sangmyung/YooHyeonJoong/05_1.pdf

		convolution(�ռ���)�� sifting(������) Ư��.

		sifting : �ܼ��� ���� Ư��.
					// ���� �ý��� �̷��� �ٿ��� �����̴�.
					// convolution���� 180�� ȸ���ϴ� ����.



*/