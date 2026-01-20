#ifndef GRAYSCALE_H
#define GRAYSCALE_H



#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

#define _USE_MATH_DEFINES  // 상수 사용을 위해서 math.h파일 이전에 정의한다.
#include <math.h>

#include "../../allocTable.h" // 상위 경로로 가기 위해서 "../" 사용한다.


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
#define BORDER_CONSTANT 0		// padding 추가 공간을 모두 0으로 채운다. (zero-padding)
#define BORDER_REPLICATE 1		// 가장자리 화소의 값으로 채우며, 꼭짓점의 화소는 인근 화소의 평균으로 채운다.
#define BORDER_REFLECT 2		// 구현하지 않음.
#define BORDER_REFLECT_101 3	// 구현하지 않음.
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

//ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ
//ㅡ code simplification
//ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ

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





//ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ
//ㅡ spatial filtering
//ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ

/*
	normalizeArray

	이전의 _buffer를 해제하며, 재가공 시 데이터가 깨질 수 있다.
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


	// _buffer의 min, max를 찾는다.
	float sourceMin, sourceMax;
	sourceMin = sourceMax = (float)_buffer[0];

	for (size_t i = 0; i < _elementCount; ++i)
	{
		sourceMin = MIN((float)_buffer[i], sourceMin);
		sourceMax = MAX((float)_buffer[i], sourceMax);
	}

	
	uint32_t* normalizedBuffer = (uint32_t*)myMalloc(sizeof(uint32_t) * _elementCount);


	// 정규화 코드에서 ratio의 분모가 0일 때 예외 처리
	if (sourceMax == sourceMin)
	{
		memcpy(normalizedBuffer, _buffer, sizeof(uint32_t) * _elementCount);
	}
	// 반올림 포함 정규화
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

	이전의 _buffer를 해제하며, 재가공 시 데이터가 깨질 수 있다.
	HPF를 거치는 embossing 전용이며, 모든 윤곽의 비율을 살리고 편향값은 127or128이다.
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



	// 버퍼의 무작위 위치에 0 또는 255의 값을 가지는 점을 _noiseCount개 찍는다.
	// 이 값을 점이 겹치지 않는 조건으로 사용하며, 점을 찍으면 개수를 샌다.
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

	filter에서 사용하고 꼭 해제하세요. (valid padding도 source를 복사하여 새로운 객체로 나갑니다.)
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

			// replicate: 코너를 별도로 처리하지 않고, 가장자리 부분을 상하-좌우 순서에 최대 폭으로 복사하면 코너 부분도 끝 픽셀 값으로 복사되는 방법을 사용한다.
			// 상
			for (y = 0; y < paddingSize; ++y)
				for (x = 0; x < dst.width; ++x)
					dst.pBuffer[y * dst.width + x] = dst.pBuffer[paddingSize * dst.width + x];
			// 하
			for (y = dst.height - paddingSize; y < dst.height; ++y)
				for (x = 0; x < dst.width; ++x)
					dst.pBuffer[y * dst.width + x] = dst.pBuffer[(dst.height - 1 - paddingSize) * dst.width + x];
			// 좌
			for (y = 0; y < dst.height; ++y)
				for (x = 0; x < paddingSize; ++x)
					dst.pBuffer[y * dst.width + x] = dst.pBuffer[y * dst.width + paddingSize];
			// 우
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
	// _src대신, _src를 padding한 src를 사용한다. _paddingMode에 따라서 원본 src보다 크기가 클 수도 있다.
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
	// buffer_padding 생성.
	float* padding = Padding(_Src, _maskConst, _paddingConst);



	// 결과 영상 생성.
	Grayscale output = { NULL, _Src.width, _Src.height };
	output.pBuffer = (unsigned char*)myMalloc(sizeof(unsigned char) * output.width * output.height);



	// 연산 편의성을 위한 buffer_padding의 width/height 정의.
	const int L = (_maskConst - 1) / 2;
	const int width_padding = (int)_Src.width + 2 * L;
	const int height_padding = (int)_Src.height + 2 * L;



	// mean filter
	// kernel의 계수가 모두 1이므로, mask를 거치지 않는다.
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



	// buffer_padding을 해제하고, 결과를 반환한다.
	subAlloc(padding);

	return output;
}





//void gaussian_filter(const Grayscale _Src, const int _maskConst, const int _paddingConst) { /*미구현*/ }





//void bilateral_filter(const Grayscale _Src, const int _maskConst, const int _paddingConst) { /*미구현*/ }




/*
	min filter
*/
Grayscale min_filter(const Grayscale _Src, const int _maskConst, const int _paddingConst)
{
	// buffer_padding 생성.
	float* padding = Padding(_Src, _maskConst, _paddingConst);



	// 결과 영상 생성.
	Grayscale output = { NULL, _Src.width, _Src.height };
	output.pBuffer = (unsigned char*)myMalloc(sizeof(unsigned char) * output.width * output.height);



	// 연산 편의성을 위한 buffer_padding의 width/height 정의.
	const int L = (_maskConst - 1) / 2;
	const int width_padding = (int)_Src.width + 2 * L;
	const int height_padding = (int)_Src.height + 2 * L;



	// min filter
	// 정렬과 mask를 거치지 않고 삼항연산자를 사용하여 바로 최솟값을 선택한다.
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



	// buffer_padding을 해제하고, 결과를 반환한다.
	subAlloc(padding);

	return output;
}




/*
	max filter
*/
Grayscale max_filter(const Grayscale _Src, const int _maskConst, const int _paddingConst)
{
	// buffer_padding 생성.
	float* padding = Padding(_Src, _maskConst, _paddingConst);



	// 결과 영상 생성.
	Grayscale output = { NULL, _Src.width, _Src.height };
	output.pBuffer = (unsigned char*)myMalloc(sizeof(unsigned char) * output.width * output.height);



	// 연산 편의성을 위한 buffer_padding의 width/height 정의.
	const int L = (_maskConst - 1) / 2;
	const int width_padding = (int)_Src.width + 2 * L;
	const int height_padding = (int)_Src.height + 2 * L;



	// max filter
	// 정렬과 mask를 거치지 않고 삼항연산자를 사용하여 바로 최댓값을 선택한다.
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



	// buffer_padding을 해제하고, 결과를 반환한다.
	subAlloc(padding);

	return output;
}




/*
	midpoint filter
*/
Grayscale midPoint_filter(const Grayscale _Src, const int _maskConst, const int _paddingConst)
{
	// buffer_padding 생성.
	float* padding = Padding(_Src, _maskConst, _paddingConst);



	// 결과 영상 생성.
	Grayscale output = { NULL, _Src.width, _Src.height };
	output.pBuffer = (unsigned char*)myMalloc(sizeof(unsigned char) * output.width * output.height);



	// 연산 편의성을 위한 buffer_padding의 width/height 정의.
	const int L = (_maskConst - 1) / 2;
	const int width_padding = (int)_Src.width + 2 * L;
	const int height_padding = (int)_Src.height + 2 * L;



	// mid-point filter
	// 정렬과 mask를 거치지 않고 삼항연산자를 사용하여 바로 최솟값과 최댓값을 구하고, 이 둘의 평균을 선택한다.
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



	// buffer_padding을 해제하고, 결과를 반환한다.
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
			// _source를 mask에 복사
			int32_t maskIndex = 0;
			for (int32_t j = -paddingSize; j <= paddingSize; ++j)
			{
				for (int32_t i = -paddingSize; i <= paddingSize; ++i)
				{
					int32_t bufferIndex = (y + j) * (int32_t)src.width + (x + i);
					mask[maskIndex++] = src.pBuffer[bufferIndex];
				}
			}

			// mask 정렬
			selectionSort(mask, maskArea);

			// 중간값 선택
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
	// buffer_padding 생성.
	float* padding = Padding(_Src, _maskConst, _paddingConst);



	// 결과 영상 생성.
	Grayscale output = { NULL, _Src.width, _Src.height };
	output.pBuffer = (unsigned char*)myMalloc(sizeof(unsigned char) * output.width * output.height);



	// 연산 편의성을 위한 buffer_padding의 width/height 정의.
	const int L = (_maskConst - 1) / 2;
	const int width_padding = (int)_Src.width + 2 * L;
	const int height_padding = (int)_Src.height + 2 * L;



	/*
		alpha-trimmed mean filter
		알파-절삭 후 남은 mask배열의 평균값을 선택한다.
	*/
	// mask배열 생성.
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


	// buffer_padding과 buffer_mask를 해제하고, 결과를 반환한다.
	subAlloc(padding);
	subAlloc(mask);

	return output;
}




/*
	low-pass linear filter
*/
Grayscale LowPass_linear_filter(const Grayscale _Src, const int _maskConst, const int _paddingConst, const float* _kernel)
{
	// buffer_padding 생성.
	float* padding = Padding(_Src, _maskConst, _paddingConst);



	// 결과 영상 생성.
	Grayscale output = { NULL, _Src.width, _Src.height };
	output.pBuffer = (unsigned char*)myMalloc(sizeof(unsigned char) * output.width * output.height);



	// 연산 편의성을 위한 buffer_padding의 width/height 정의.
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



	// buffer_padding을 해제하고, 결과를 반환한다.
	subAlloc(padding);

	return output;
}




/*
	high-pass linear filter
*/
Grayscale HighPass_linear_filter(const Grayscale _Src, const int _maskConst, const int _paddingConst, const float* _kernel)
{
	// buffer_padding 생성.
	float* padding = Padding(_Src, _maskConst, _paddingConst);



	// 결과 영상 생성.
	Grayscale output = { NULL, _Src.width, _Src.height };
	output.pBuffer = (unsigned char*)myMalloc(sizeof(unsigned char) * output.width * output.height);



	// 연산 편의성을 위한 buffer_padding의 width/height 정의.
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



	// buffer_padding을 해제하고, 결과를 반환한다.
	subAlloc(padding);

	return output;
}




/*
	embossing filter
*/
Grayscale embossing_filter(const Grayscale _Src, const int _maskConst, const int _paddingConst, const float* _kernel)
{
	// buffer_padding 생성. 추가로 정규화를 위한 buffer_TEMP도 생성.
	float* padding = Padding(_Src, _maskConst, _paddingConst);
	float* TEMP = (float*)myMalloc(sizeof(float) * _Src.width * _Src.height);


	// 결과 영상 생성.
	Grayscale output = { NULL, _Src.width, _Src.height };
	output.pBuffer = (unsigned char*)myMalloc(sizeof(unsigned char) * output.width * output.height);



	// 연산 편의성을 위한 buffer_padding의 width/height 정의.
	const int L = (_maskConst - 1) / 2;
	const int width_padding = (int)_Src.width + 2 * L;
	const int height_padding = (int)_Src.height + 2 * L;



	// embossing filter (편향값 128)
	// 고주파만 생성한다.  ->  [-128, 127] 범위로 정규화한다.  ->  편향값으로 + 128을 더한다.
	// 결국에 가장 강한 상승고주파가 255의 값을 가지고, 가장 강한 하강고주파가 0의 값을 가지게 된다.
	// 고주파 추출.
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



	// [-128, +127] 범위로 정규화.
	TEMP = (float*)normalizeArray_embossing(TEMP, output.width * output.height, 0, 255, "float");


	int iLoop = (int)(output.width * output.height);
	for (i = 0; i < iLoop; ++i)
	{
		output.pBuffer[i] = (unsigned char)TEMP[i];
	}



	// buffer_padding을 해제하고, 결과를 반환한다.
	subAlloc(padding);

	return output;
}





//ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ
//ㅡ binarization
//ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ

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





//ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ
//ㅡ histogram processing
//ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ

/*
	getLUT_with_straight

	[ 두 점의 x, y좌표 (x1, x2, y1, y2) ]
	두 점을 이은 연속함수 그래프의 LUT를 생성하고 반환한다.
	정의역(x1부터 x2까지의 범위)를 벗어난 부분은, x축과 평행한 선분이다.
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
	[ 원본 영상 ]
	원본 영상에 대한 가공(정규화)되지 않은 histogram data를 생성하고, 이를 반환한다.
	원본 영상 _Src의 buffer를 해제하지 않는다.
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

	[0, 256]로 정규화된 히스토그램을 받고, 256*256의 영상으로 반환한다.
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
	[ 원본 영상 ]
	원본 영상에 대한 histogram image를 생성하고, grayscale형 객체로 반환한다.
	원본 영상 _Src의 buffer를 해제하지 않는다.
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
	[ 원본 영상 ] . [ LUT ]
	원본 영상을 LUT를 통해 equalizing하고, grayscale형 객체로 반환한다.
	_Src의 buffer와 _LUT를 해제하지 않는다.
*/
Grayscale equalize_with_grayscale(Grayscale _Src, unsigned char* _LUT)
{
	Grayscale EQ_Src = { NULL, _Src.WIDTH, _Src.HEIGHT };
	EQ_Src.pBuffer = (unsigned char*)myMalloc(sizeof(unsigned char), EQ_Src.WIDTH * EQ_Src.HEIGHT);


	int i, iLoop = (int)(EQ_Src.WIDTH * EQ_Src.HEIGHT);
	for (i = 0; i < iLoop; ++i)
	{
		// WARNING : buffer overrun
		// compiler가 인식을 못해서 내세운 잠재적 경고가 뜰 수 있다.
		// index연산이 확실하므로, 무시할 것.
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
	[ 원본 영상 ]
	원본 영상의 histogram data를 통해 LUT를 생성하고 반환한다.
	원본 영상 _Src의 buffer를 해제하지 않는다.
*/
unsigned char* getLUT_with_grayscale(Grayscale _Src)
{
	int* hist1D = extractHistogram(_Src);

	hist1D = accumulateArray(hist1D, _8bit);

	unsigned char* LUT_Src = (unsigned char*)normalizeArray(hist1D, _8bit, 0, 255, "unsigned char");

	return LUT_Src;
}





//ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ
//ㅡ geometric processing
//ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ

/*
	shift

	[ input ] . [이동할 x좌표값] . [y좌표값]
	원본 _Src의 buffer를 해제하지 않는다.
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
	원본 영상의 중심(float)을 원점으로 회전한다.
	원본 _Src의 buffer를 해제하지 않는다.
	bilinear interpolation, reverse mapping.
*/
Grayscale rotation_bilinear_interpolation(Grayscale _Src, int _degree)
{
	// 좌표 역변환 구현에서의 연산 오차로 인해 input의 index범위를 벗어난 할당되지 않은 화소값을 읽을 수 있으나,
	// 예외 처리 1번에서 오차를 우선 허용하고 2번과 3번에서 다시 걸러내어 결국 이론과 같은 결과를 도출한다.
	// 읽는 것 자체가 문제가 된다면, input buffer의 상하좌우 한 줄을 늘려서 input 대신 사용하면 된다.



	// 출력 객체 생성.
	Grayscale Dst = { NULL, _Src.width, _Src.height };
	Dst.pBuffer = (unsigned char*)myCalloc(sizeof(unsigned char) * Dst.width * Dst.height);



	// rotation에 사용할 상수들.
	const float radian = (float)(_degree % 360) * (float)M_PI / 180.f;
	const float COS = cosf(radian);
	const float SIN = sinf(radian);

	// 좌표 역변환 연산에서의 허용 오차.
	const float R = 0.0001f;



	// Dst 중심점. ( == _Src 중심점.)
	const float Xcenter = ((float)_Src.width - 1.f) / 2.f;
	const float Ycenter = ((float)_Src.height - 1.f) / 2.f;



	// Dst 기준 좌표.
	int Xdst, Ydst;

	// _Src 기준 좌표. (Dst 좌표의 역변환을 통해 구한다.)
	float Xsrc, Ysrc;



	// 보간 가중치 알파/베타.
	float a; // alpha (x-axis)
	float b; // beta (y-axis)

	// 보간값을 보관할 임시 변수.
	float TEMP;



	// rotation with bilinear interpolation.
	for (Ydst = 0; Ydst < Dst.height; ++Ydst)
	{
		for (Xdst = 0; Xdst < Dst.width; ++Xdst)
		{

			// 역변환을 통해 _Src좌표를 구한다.
			Xsrc = (Xdst - Xcenter) * COS + (Ydst - Ycenter) * SIN + Xcenter;
			Ysrc = -(Xdst - Xcenter) * SIN + (Ydst - Ycenter) * COS + Ycenter;



			// 예외 처리 1 : Dst좌표를 역변환한 _Src좌표가 index범위를 벗어난다면, 현재 loop를 건너뛴다.
			// (소수점 일부가 잘린 값들로 연산하여 오차가 발생하므로, 허용 오차 R값을 적용한다.)
			// (이 오차는 무의미할 정도로 작지만, 연산 결과가 이론상 정수인 동시에 index범위에 해당되고 buffer의 가장자리일 때 통과할 수 없음을 방지한다.)
			if (-R > Xsrc || (float)(_Src.width - 1) + R < Xsrc || -R > Ysrc || (float)(_Src.height - 1) + R < Ysrc)
				continue;



			// 보간을 위해 _Src의 현재 좌표에 대한 가중치 alpha/beta값을 구한다.
			a = Xsrc - (int)Xsrc;
			b = Ysrc - (int)Ysrc;



			// 예외 처리를 위해 보간값을 임시 저장한다.
			// 예외 처리 2번과 3번은 linear interpolation(선형 보간)의 구현에서 발생할 수 있는 문제이므로, 해결해야 한다.
			TEMP =
				(1.f - a) * (1.f - b) * (float)_Src.pBuffer[(int)_Src.width * (int)Ysrc + (int)Xsrc] +
				a * (1.f - b) * (float)_Src.pBuffer[(int)_Src.width * (int)Ysrc + ((int)Xsrc + 1)] +
				(1.f - a) * b * (float)_Src.pBuffer[(int)_Src.width * ((int)Ysrc + 1) + (int)Xsrc] +
				a * b * (float)_Src.pBuffer[(int)_Src.width * ((int)Ysrc + 1) + ((int)Xsrc + 1)];



			// 예외 처리 2 : 초과. (인접화소가 모두 0 또는 255인 선형 보간에서 연산 오차에 의해 발생할 수 있으며, 이외에는 절대로 발생할 수 없다.)
			if (255.f < TEMP)
				TEMP = 255.f;
			if (0.f > TEMP)
				TEMP = 0.f;



			// 예외 처리 3 : 반올림 + 선형 보간에서의 오차 처리.
			// (보간이 필요 없는 90도 단위의 회전도 보간을 거침으로써 0.001 이하의 오차가 발생한다. e.g.[lena영상의 +90도 회전 : 오류 4915개])
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
	원본 영상의 중심(float)을 원점으로 회전한다.
	원본 _Src의 buffer를 해제하지 않는다.
	nearest neighbor interpolation, reverse mapping.
*/
Grayscale rotation_nearest_neighbor_interpolation(Grayscale _Src, int _degree)
{
	// 출력 객체 생성.
	Grayscale Dst = { NULL, _Src.width, _Src.height };
	Dst.pBuffer = (unsigned char*)myCalloc(sizeof(unsigned char) * Dst.width * Dst.height);



	// rotation에 사용할 상수들.
	const float radian = (float)(_degree % 360) * (float)M_PI / 180.f;
	const float COS = cosf(radian);
	const float SIN = sinf(radian);

	// 좌표 역변환 연산에서의 허용 오차.
	const float R = 0.0001f;



	// Dst 중심점. ( == _Src 중심점.)
	const float Xcenter = ((float)_Src.width - 1.f) / 2.f;
	const float Ycenter = ((float)_Src.height - 1.f) / 2.f;



	// Dst 기준 좌표.
	int Xdst, Ydst;

	// _Src 기준 좌표. (Dst 좌표의 역변환을 통해 구한다.)
	float Xsrc, Ysrc;



	// 보간 가중치 알파/베타.
	float a; // alpha (x-axis)
	float b; // beta (y-axis)



	// rotation with nearest neighbor interpolation.
	for (Ydst = 0; Ydst < Dst.height; ++Ydst)
	{
		for (Xdst = 0; Xdst < Dst.width; ++Xdst)
		{

			// 역변환을 통해 _Src좌표를 구한다.
			Xsrc = (Xdst - Xcenter) * COS + (Ydst - Ycenter) * SIN + Xcenter;
			Ysrc = -(Xdst - Xcenter) * SIN + (Ydst - Ycenter) * COS + Ycenter;



			// 예외 처리 1 : Dst좌표를 역변환한 _Src좌표가 index범위를 벗어난다면, 현재 loop를 건너뛴다.
			// (소수점 일부가 잘린 값들로 연산하여 오차가 발생하므로, 허용 오차 R값을 적용한다.)
			// (이 오차는 무의미할 정도로 작지만, 연산 결과가 이론상 정수인 동시에 index범위에 해당되고 buffer의 가장자리일 때 통과할 수 없음을 방지한다.)
			if (-R > Xsrc || (float)(_Src.width - 1) + R < Xsrc || -R > Ysrc || (float)(_Src.height - 1) + R < Ysrc)
				continue;



			// 보간을 위해 _Src의 현재 좌표에 대한 가중치 alpha/beta값을 구한다.
			a = Xsrc - (int)Xsrc;
			b = Ysrc - (int)Ysrc;



			// 인접 화소값을 그대로 가져오는 비선형 보간이므로, 예외 처리가 없다.
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
	원본 _Src의 buffer를 해제하지 않는다.
	bilinear interpolation, reverse mapping.
*/
Grayscale zoomIn_bilinear_interpolation(const Grayscale _Src, const size_t _DstWidth, const size_t _DstHeight)
{
	/*
		_Src에 bilinear interportion을 적용하기 위해서는, input.pbuffer의 우단/하단 Line이 한 줄씩 더 필요하다.
		_Src.width = _Src.width + 1
		_Src.height = _Src.height + 1
		입력 _Src.pbuffer를 input.pbuffer에 복사하고, 추가 공간은 이웃한 화소의 값으로 대체한다.
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
	// 이제 _Src.pbuffer 대신에 input.pbuffer를 참조하여 보간하면 된다.



	// output 생성.
	Grayscale output = { NULL, _DstWidth, _DstHeight };
	output.pBuffer = (unsigned char*)myMalloc(sizeof(unsigned char) * output.width * output.height);



	// bilinear interpolation.
	// 좌표 (i, j)는 output을 기준으로 한다.
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



	// _Src.pbuffer 대신 사용한 input.pbuffer를 해제하고, output을 반환한다.
	subAlloc(input.pBuffer);
	return output;
}





/*
	zoomIn_nearest_neighbor_interpolation

	[ input ] . [ output.width ] . [ output.height ]
	원본 _Src의 buffer를 해제하지 않는다.
	nearest neighbor interpolation, reverse mapping.
*/
Grayscale zoomIn_nearest_neighbor_interpolation(const Grayscale _Src, const size_t _DstWidth, const size_t _DstHeight)
{
	/*
		_Src에 bilinear interportion을 적용하기 위해서는, input.pbuffer의 우단/하단 Line이 한 줄씩 더 필요하다. (사실 계산을 해 봐야 한다.)
	 
		입력 영상의 가로 길이가 a, 출력 영상의 가로 길이가 b라 할 때,
		Xpct = a/b
		"(b - 1) * Xpct >= (a - 1) + 0.5"
		위 식을 만족시키는 자연수 쌍 (a, b)가 존재하는가??
		정리 -> "b >= 2 * a"
		무수히 많이 존재하며, 세로 길이 또한 마찬가지이므로, 결국 Line을 하나씩 추가해야 한다.
	 
		_Src.width -> _Src.width + 1
		_Src.height -> _Src.height + 1
		입력 _Src.pbuffer를 input.pbuffer에 복사하고, 추가 공간은 이웃한 화소의 값으로 대체한다.
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
	// 이제 _Src.pbuffer 대신에 input.pbuffer를 참조하여 보간하면 된다.



	// output 생성.
	Grayscale output = { NULL, _DstWidth, _DstHeight };
	output.pBuffer = (unsigned char*)myMalloc(sizeof(unsigned char) * output.width * output.height);



	// nearest neighbor interpolation.
	// 좌표 (i, j)는 output을 기준으로 한다.
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



	// _Src.pbuffer 대신 사용한 input.pbuffer를 해제하고, output을 반환한다.
	subAlloc(input.pBuffer);
	return output;
}





/*
	TEST_zoonInOut

	Xpct/Ypct가 1 이상이 되는 "축소"일 경우에,
	평균과 보간을 모두 적용하며,
	Xpct/Ypct가 1 미만일 때도 똑같이 적용된다.
*/
Grayscale TEST_zoonInOut(const Grayscale _Src, const size_t _DstWidth, const size_t _DstHeight)
{
	/*
		참고 : https://cho001.tistory.com/146

		11.22 과제
		1. 256*256 영상을 64*64로 축소
		2. 1의 영상을 256*256으로 확대
		3. 1의 영상을 300*300으로 확대
		4. 임의의 영상을 N*N으로 확대. (임의 확대)

		5. 임의의 각도로 회전.



		확대에 대해서.
			순방향 사상, 역방향 사상.
			채우려는 것은 확대 영상이다.
			확대영상의 idx를 채워나가되,
			src영상을 참고만 할 뿐이다.
			이는 역방향 사상이다.
			사실 우리가 하고 있던 것들이다.


		4배 줄이고 4배 늘리면
		원본이 나와야 한다.
		그러려면,
		확대축소 함수가 원본의 정보를 매번 받거나, 받았을 때 저장해두어야 한다.
		파워포인트에 사진 넣고 확대축소해도 안변하는건, 처음 원본의 정보를 가지고 있기 때문이다.


		보간법
			idx가 21.3이라면,
			idx21의 70%랑 idx22의 30%를 가져오면 되겠다.
			y축도 똑같이 적용하면 될 것 같다.
			이를 양방향 선형보간(bilinear interpolation)이라고 한다.
			연산 없이 반올림해서 인접픽셀(nearless neighborhood???)을 가져오는 방법도 있다.


		조건
		1.
		zoom-in/zoom-out 함수는 global object를 통하여 origin의 정보를 받으면 저장하되,
		연산 또한 항시 origin 기준으로 진행하고 return할 것.

		2.
		zoom-in/zoom-out 함수에 적용할 여러 가지 보간법은, 별도의 함수로 정의하고 사용할 것.
		확대/축소 뿐만 아니라, 회전 등의 기하학 변형에 사용할 수 있기 때문이다.
		// 아닌 것 같다. 가독성이 더 떨어진다. 그냥 원리만 외워두자.
	*/



	// _Src에 bilinear interportion을 적용하기 위해서는, 2Dbuffer의 우단/하단 pixel이 하나씩 더 필요하다.
	// _Src.width -> _Src.width + 1,  _Src.height -> _Src.height + 1
	// 이 추가 공간은, 이웃한 화소의 값으로 대체한다.
	// 라고 했으나,
	// 추가 공간을 이웃한 화소의 값으로 대체하는 건 똑같고,
	// 만약 10배 축소한다면, 최소 10개 이상의 추가 좌표가 필요할텐데,,

	// 확대에서는 역방향 연산이 아주 편하지만,
	// 축소에서 역방향 연산을 하기 위해서 보간과 평균을 동시에 하려니, 아주 복잡해졌다.
	// 만약에,
	// width/height 중 하나는 축소, 하나는 확대라면??
	// 역방향과 정방향을 동시에 가능할까??

	// 가능하다면, 축소 배율에 상관 없이 _Src.pbuffer의 추가 공간을 +1로 고정시킬 수 있다.

	// 가능한데, 유지/보수는 포기해야 하고, 나는 역방향/정방향 혼합이 더 복잡하다.

	// 그냥 처음 생각한대로 역방향으로 다 해결하되, if조건을 조금 많이 첨가하자.
	// 당연히 추가 공간은 1로 고정시켰으니, 마지막 idx에 달하면 그만하자는 것도 if조건으로 두고.



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
	// 이제 _Src.pbuffer 대신에 input.pbuffer를 참조하여 보간하면 된다.



	// output 생성.
	Grayscale output = { NULL, _DstWidth, _DstHeight };
	output.pBuffer = (unsigned char*)myMalloc(sizeof(unsigned char) * output.width * output.height);



	// output 기준 좌표.
	int X_output;
	int Y_output;

	const float Xpct = (float)_Src.width / (float)_DstWidth;
	const float Ypct = (float)_Src.height / (float)_DstHeight;


	// output 기준 좌표에 대한 input 좌표.
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



	// _Src.pbuffer 대신 사용한 input.pbuffer를 해제하고, output을 반환한다.
	subAlloc(input.pBuffer);
	return output;
}





/*
	TEST_TEST_zoomInOut

	확대축소 모두 가능하게 보간법이랑 평균 모두 활용. 베이스는 reverse mapping. 1D로 구현중.
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

	// 배열의 모든 요소를 순회
	for (i = 0; i < _count - 1; i++) {
		// 현재 위치에서 최솟값의 인덱스를 찾음
		min_idx = i;
		for (j = i + 1; j < _count; j++) {
			if (_arr[j] < _arr[min_idx]) {
				min_idx = j;
			}
		}

		// 최솟값을 현재 위치와 교환
		if (min_idx != i) {
			SWAP(_arr[i], _arr[min_idx], temp);
		}
	}
}




#endif // GRAYSCALE_H
















/*
ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ
ㅡ note
ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ

*** 이 주석은, [프로그래밍언어2]과목에서 도대체 내가 뭘 배우고 있는지 궁금하여, 디지털 영상 처리의 용어들을 알고리즘 방식에 따라 분류한 것이다.

*** 디지털 영상 처리의 대분류만을 목적으로 작성한 주석이며, 신뢰할 수 있지만? 공백이 많고 완성되지 않았다.

***	표시된 참고 사이트는, 보다 완벽한 글이어서 요약하지 않은 글이거나, 아직 정리를 하지 못한 글이거나, 복잡한 수학적 공식 또는 자세한 원리에 대한 참조 글이다.

***	spatial filtering(공간 필터링)에 대해서는 image domain filtering(영상 공간 필터링)만 다루며, frequency domain filtering(주파수 공간 필터링)은 다루지 않았다.
	image processing(영상 처리 기법)  ->  spatial domain processing(공간 도메인 연산), frequency domain processing(주파수 도메인 연산)

***	이 글은, 영상처리의 아주 좁은 범위이며 대부분의 세부 과정이 생략되어 있으나, 출처는 남겼다. 근데 봐도 모른다.

***	mask 혹은 kernel을 거친 pixel값이 어떻게 변하는지에 초점을 둘 것. 이는 filter의 종류를 분별하는데 중요한 역할을 한다.

***	구현할 filter에 대한 계산 복잡도는 알아서 판단할 것.

*** 미적분과 같은 수학적 접근이 어렵다면, < Histogram Modification > 파트에 있는 확률변수 용어와 연속/이산 그래프로 접근하여 kernel을 쉽게 구할 수 있다.
	HPF의 이산미분 그래프의 예시가 있다. 참고 : https://everyday-image-processing.tistory.com/142
	어차피 각 filter별로 kernel의 종류는 많지 않으므로, 실습을 통한 kernel들의 비교가 가능하다면, 과정은 괜찮다 생각한다.
	하지만 필요에 의해 customizing 구현하는 것은 불가능할 것이다.
	물론 나도 안했다.







	< digital image processing > (디지털 영상 처리 / 디지털 화상 처리)
		// 참고 : https://typoon.tistory.com/m/14

		디지털 영상 처리 기술에 따른 분류
			Image Enhancement (영상 개선)
			Image Restoration (영상 복원)
			Image Transformation (영상 변환)
			Image Analysis (영상 분석)
			Image Understanding (영상 인식)
			Image Compression (영상 압축)

		디지털 영상 처리 알고리즘에 따른 분류
			Pixel Point Processing (화소 점 처리, 단일 픽셀 처리)
			Pixel Area(Group) Processing (화소 영역 처리, 픽셀 그룹 처리)
			Geometric Processing (기하학적 처리)
			Frame Processing (프레임 처리)







	< interpolation > (보간, 보간법)
	@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ 정리하자.

		// 







	< Geometric Processing > (기하학적 처리)

			ㅡ mapping (사상)
				// 참고 : https://cho001.tistory.com/146
				// 참고 : C:\Users\bibib\OneDrive\바탕 화면\코딩과제\13주차_과제\RESULT.PNG

					ㅡ forward mapping (순방향 사상) : 입력 영상의 좌표를 기준으로 처리한다.

					ㅡ reverse mapping (역방향 사상) : 출력 영상의 좌표를 기준으로 처리한다.

						입력/출력의 크기가 같다면 상관없지만, 그렇지 않다면 hole(홀)이나 overlap(오버랩)의 문제가 발생할 수 있다.



					ㅡ rotation (회전)
						// 참고 : https://cho001.tistory.com/146
						// 참고 : https://m.blog.naver.com/PostView.naver?isHttpsRedirect=true&blogId=junhyuk7272&logNo=220140840595
						// 참고 : https://color-change.tistory.com/54
						// 증명은 참고에 있으며, 공식만 적었다.

							회전은 입력 영상의 모든 화소를, 입력의 원하는 원점 좌표를 기준으로 원하는 각도만큼 모든 화소에 대해 회전 변환하는 것을 말한다.

							입력 좌표가 (X, Y), 회전 변환된 좌표가 (X', Y')이고
							원점 좌표를 (0, 0)이라 하면,

								ㅡ forward mapping	  :	X' = Xcos(θ) - Ysin(θ)
														Y' = Xsin(θ) + Ycos(θ)

								ㅡ reverse mapping	  :	X = X'cos(θ) + Y'sin(θ)
														Y = -X'sin(θ) + Y'cos(θ)


							원점 좌표를 (Xctr, Yctr)라고 하면,

								ㅡ forward mapping	  :	X' = (X - Xctr)cos(θ) - (Y - Yctr)sin(θ) + Xctr
														Y' = (X - Xctr)sin(θ) + (Y - Yctr)cos(θ) + Yctr

								ㅡ reverse mapping	  :	X = (X' - Xctr)cos(θ) + (Y' - Yctr)sin(θ) + Xctr
														Y = -(X' - Xctr)sin(θ) + (Y' - Yctr)cos(θ) + Yctr

															

					ㅡ 회전 변환 연산 오차에 관하여. (rotation() 함수)

							결론 : float형으로 연산할 경우, 90도 단위의 회전 변환에서 0.0000076 ~ 0.0000077 사이의 오차가 생겼다. double형으로 연산할 경우, 오차는 더 작아질 것이다.
									이렇게 작은 오차가 문제를 발생시켰다.


							rotation() 함수는 reverse mapping으로 output좌표값에 이와 대응되는 input 역변환 좌표값을 복사하는 방법으로 구현되었고, 양선형 보간을 포함한다.
							output좌표를 역변환한 input에서의 좌표가 index범위를 벗어난다면, 해당 output좌표는 건너뛴다.

							이 index범위를 벗어날 때 건너뛴다는 예외 처리에서 문제가 생긴다.



							256x256 영상에서 x와 y의 범위는 다음과 같다.
								0 <= x <= 255
								0 <= y <= 255
							중심점을 정확히 127.5로 잡고 90도 단위로 회전할 때
							양선형 보간의 알파/베타 값은 항상 0으로 보간이 필요가 없어야 하는데,
							65536개 중 82개의 pixel이 input의 index범위를 벗어난다. 즉, 가장자리 일부분이 출력되지 않는다.

							원인은 아래와 같다.
								ㅡ	math.h의 원주율, 삼각함수 등의 정의/반환값은 실제 값이 아니므로 오차가 존재한다.
									처음부터 오차가 존재하는 값들로 연산을 했으니, 연산 결과 또한 당연히 오차가 존재한다.
									이 오차는 무의미할 정도로 작지만, 연산 결과가 이론상 정수여야할 때 문제가 발생한다.

							근본적인 문제인 오차를 해결하는 방법으로 한 가지가 있다. 이것 외에는 사실상 불가능하다.
								ㅡ	계산기와 같은 calculation module이 필요하다. 이는 사람이 수식을 직접 계산하듯이 오차를 만들지 않을 것이다.


							오차를 해결하지 않는다면, 예외 처리로 가야 한다.
								ㅡ	x와 y의 범위를 다음과 같이 한다.
										R = 0.00001f
										-R <= x <= 255 + R
										-R <= y <= 255 + R
									또한 input의 상하좌우 한 줄을 추가하여 연산한다.
									// 추가하여 연산하지 않는다고 해도, 보간에서 index범위를 벗어나는 pixel에 대한 비중이 R값만큼 작을 것이다.
									// 그러나, 이것도 명백한 메모리 침범(buffer overflow)이다.
									// 할당된 memory만을 사용하며, 할당되지 않은 memory를 사용하지 않는 것은 매우 당연하다.







	< Pixel Point Processing > (화소 점 처리)  ==  단일 픽셀 처리

		이웃 픽셀과는 독립적으로
		입력 영상의 각 픽셀 값을 수학적인 함수를 거쳐 새로운 값으로 변환하고, 출력 영상의 동일한 위치에 저장하는 연산이다.

		화소 처리에서 각 출력 화소값은 해당 입력 화소값에만 의존한다.
		이것은 출력 화소의 값을 결정할 때, 이웃한 화소들의 값을 활용하는 공간 필터링과 비교된다.

		// 굳이 영역 처리로 끌고 온다면, mask 행렬이 [1*1]이다.

			ㅡ 산술연산, 논리연산, 그레이스케일 변환, Global Fixed Thresholding(전역 고정 이진화), Histogram Modification(히스토그램 변형), Global Histogram Processing(전역적 히스토그램 처리) 등

			ㅡ e.g.
				darken(f-a) lighten(f+a) invert(255-f) low-contrast(f/2) high-contrast(f*2) gray(0.3f_R + 0.6f_G + 0.1f_B)







	< Pixel Area(Group) Processing > (화소 영역 처리, 픽셀 그룹 처리)

		인접화소들의 값을 참조하여 화소의 값을 변경하는 처리이다.

		ㅡ Local Adaptive Thresholding(지역 가변 이진화), Local Histogram Processing(지역적 히스토그램 처리), Spatial Filtering (공간 필터링) 등







	< Padding > (패딩)
		// 참고 : https://ardino.tistory.com/40
		// 참고 : https://ardino.tistory.com/41

		// 영역 처리를 하기 전에 꼭 알아야 할 개념이다.

		// 뭐 이리 패딩의 종류가 많은지 불만이면, mask모양을 정방형이 아닌 다이아몬드, 십자가 따위로 한다 생각하면 이해가 될 것이다.


		padding (패딩) : 가장자리 화소를 filtering하기 위해서, 입력 영상의 외곽에 임시 공간과 값을 추가하는 것.
							얼만큼의 공간과 어떠한 값을 사용하는지에 따라 padding의 종류가 결정된다.
							// 화소의 연산 참여도란, 특정 화소가 mask에 몇 번 투입되었는지를 말한다.
							// 가장자리 화소는 영역 처리가 불가능하고, padding을 하지 않을 시 출력 영상의 SIZE가 감소한다.
							// padding은 이를 해결하기 위한 방법이며, 출력 전에 당연히 제외해야 한다.
							// 구현 : 일반적으로 실수형이며, 같은 실수형인 kernel과의 convolution연산이 진행된다.


		Stride (스트라이드, 보폭) : filtering에서 mask가 움직이는 거리.
									필터링은 필터를 이미지에 놓고 움직이면서 한다.
									이때 필터가 한번에 움직이는 거리를 보폭, 흔히 스트라이드(stride)라고 부른다.
									mask가 한 칸씩 움직이면 스트라이드 값은 1이 되고, 두 칸씩 움직이면 2가 된다.
									// stride 역시 출력 영상의 SIZE에 관여한다.



		ㅡ padding의 추가 공간의 크기에 따른 분류

			ㅡ Valid Padding : padding을 적용하지 않는 것.

			ㅡ Full Padding : 입력 영상의 모든 화소의 연산 참여도가 동일하도록 padding하는 것.
								// 출력 영상의 SIZE가 증가한다.
								// 정방형의 입력 영상에 대해 정방형이 아닌 mask와 full padding을 사용할 때, 출력 영상이 정방형이 아니게 되는 부분은 나도 모르겠다.
								// 아마도 정방형으로 만들기 위해서 외곽의 빈 공간에 추가 연산을 하면 될 것 같다.

			ㅡ Same Padding : 입력 영상과 출력 영상의 크기가 같도록 padding하는 것.



		ㅡ padding의 추가 공간의 값에 따른 분류
			// 참고 : https://wiserloner.tistory.com/845
			// 참고 : https://cafepurple.tistory.com/54

			대표적인 두 가지 방법이 있지만, filter마다 다를 수 있다.
			참고 사이트에 openCV에 정의되어있는 padding 관련 상수가 있다.

			1.	Zero-Padding (0-padding, 제로 패딩)
				간단하지만, 출력 영상의 가장자리 데이터가 변형/소실된다.

			2.	가장자리 값으로 padding한다.







	< Spatial Filtering > (공간 필터링)
		// 참고 : https://www.geeksforgeeks.org/spatial-filtering-and-its-types/

		Filter (필터) : 입력 신호의 일부 성분을 제거하거나 일부 특성을 변경하기 위해 설계된 하나의 시스템이다.

		공간 필터링이란, 영상 신호에 대한 공간 영역(Spatial Domain)에서의 필터 처리를 의미한다.

		// 필터링의 용어는 모두 주파수 영역에서 다루는 용어를 그대로 사용한다.
		// "공간영역(spatial domain)"이라 칭하는 이유는 "주파수 영역(frequency domain)"과 차이를 두기 위해서이다.
		// 화소들이 존재하는 2차원 공간에서 필터링을 수행한다는 의미이다.

		// "spatial filtering"은, "Local Histogram Processing"과 내부 연산이 다른 것 빼고 유사하다.


		Mask (마스크) : filtering의 성격을 정의하는 2차원 행렬이다.
						mask에 적용되는 연산의 형태는, filtering의 결과를 결정하고 filter의 종류를 결정한다.
						// kernel(커널), window(윈도우), template(템플릿)이라고도 부르며, mask 자체를 filter라고 부르기도 한다.
						// 혹은 원본 영상의 특정 위치에 대한 filtering이 진행되는 공간을 mask라 하기도 한다.
						// 이때의 공간이란, { (filtering하기 위한 영상 원본의 일부분), (mask), (적용할 연산) }이다.


		Kernel (커널) : linear spatial filtering(선형 공간 필터링)의 convolution연산에 사용되는 가중치 행렬이다.
						// 구현 : padding이 적용된 실수형 buffer와 convolution연산이 진행되므로, kernel은 실수형으로 한다.



		filter의 선형성에 따른 분류
			Linear Spatial Filter (선형 공간 필터) : convolution연산을 사용하는 filter이다.
			Nonlinear Spatial Filter (비선형 공간 필터) : convolution연산을 사용하지 않고, 정렬 등의 다른 연산을 사용하는 filter이다.

		또 다른 일반적 분류
			smoothing spatial filter(스무딩 공간 필터)  ->  smoothing linear filter(스무딩 선형 필터), order-statistic filter(순서-통계 필터)

		비선형 공간 필터와 순서-통계 필터는 같다고 보면 된다.



		***	1.	α-trimmed Mean Filter(α-절삭 평균값 필터)는 평균값 필터와 중간값 필터의 결과를 포함하므로, 두 필터를 생략할 수 있다.
		***	2.	linear filter는 가중치 행렬인 kernel의 종류에 의해서만 분류되므로,
		***		kernel을 인자로 받는 단 하나의 linear_filter() 통합함수로 구현할 수 있다.
		***	3.	필요에 의해 linear, nonlinear 연산을 모두 사용하는 filter를 구현할 수 있다.
		***	하지만,
		***	명칭이 존재하고 잘 알려진 filter, 혹은 filter 분류의 이름은 각자의 정해진 목적 또는 장점, 쓰임새와 알고리즘이 있으며,
		***	이들을 customizing한 filter를 같은 이름으로 구현하여 사용해선 안 된다는 것을 기억해야 한다.



		Convolution (컨볼루션, 합성곱)  :  중심 화소의 값을 중심을 포함한 인접 화소값들의 가중 합(weighted sum)으로 대체하는 연산이다.
		Correlation (코릴레이션, 상관)		kernel이 가중치(weight)로 사용된다.

											mask 가중치의 합(kernel 원소의 총합)은, 출력 영상의 전체적인 밝기에 영향을 준다.
											값이 1일 때, 원본의 밝기가 유지된다.
											// edge-detection(윤곽선 검출)에서는 0이라 보면 된다.

											// linear filter에 사용되는 연산으로 correlation(코릴레이션)과 convolution(컨볼루션, 합성곱)이 있다.
											// correlation : SOP(sum of products)와 동일하다.
											// convolution : origin 혹은 kernel을 180도 회전하여 correlation을 적용한다.  // 왜 180도 회전하는지는 글의 마지막 미처리건에 있다.

											// correlation, convolution 모두를 SOP라는 의미로 통틀어서 convolution이라 부르기도 한다.

											// 이 글에서 linear filter의 구현에 사용되는 연산을 correlation으로 통일하였다.
											// 설명하기 위해서 구분은 하되, 용어는 대체로 convolution으로 통일하였다.

											***	연습문제.									+0  +0  +0
												입력과 출력이 동일한 filter의 kernel[3*3]  =	+0  +1  +0
																							+0  +0  +0







*** convolution연산을 사용하는 linear filter 구현에 있어서 중요한 한 가지.

			LPF에서 정확히 구현하고 정확한 kernel을 내어주었다면, 절대로 overflow가 일어나지 않는다.
			부정확한 kerenl을 내어주었음을 확인하기 위해서라도, overflow를 해결하지 않는 것이 debugging을 더 쉽게 한다.
			왜냐햐면, 부자연스러운 영상이 출력되어 오류를 확인하기가 쉽기 때문이다.

			sharpening에서는 overflow가 일어나는 것이 당연하며, 당연히 이를 해결해야 한다.
			여기서 절대로 normalizing을 해서는 안 되며, overflow를 모두 0 또는 255의 값으로 고정해야 한다.

			예시로, 가중치의 합(1)의 sharpening에서는 영상의 밝기를 보존하며 영상을 sharp하게 한다.
			그러나 overflow를 해결하기 위해서 normalizing을 거친다면,
			똑같이 출력되어야 하는 평지에서의 밝기에 영향을 끼친다.(사실상 모든 화소값이 뒤틀린다.)



			HPF에서는 overflow가 일어나는 것이 당연하며, 당연히 이를 해결해야 한다.
			해결 방법은, convolution연산에 사용된 mask 가중치의 합(kernel 원소의 총합)이 0이냐, 0이 아니냐로 나뉜다.

					1.	가중치의 합이 1일 때.
							convolution 후에 절대로 normalizing을 해서는 안되며, overflow를 모두 0 또는 255의 값으로 고정해야 한다.
							
							예시로, 가중치의 합이 1인 sharpening에서는 영상의 밝기를 보존하며 영상을 sharp하게 한다.
							그러나 overflow를 해결하기 위해서 convolution 후에 normalization을 거친다면,
							똑같이 출력되어야 하는 평지에서의 밝기에 영향을 끼친다. (사실상 모든 화소값이 뒤틀린다.)
							이를 해결하기 위해서 sharpening을 윤곽의 비율을 보존하도록 구현한다면, 이는 더 이상 sharpening이 아니게 된다.

					2.	가중치의 합이 0일 때.
							영상의 윤곽/고주파만 남게 되며, 이 정규화되지 않은 고주파를 어떻게 사용할 것인지는 구현하고자 하는 filter에 따라 다르다.

							embossing의 전체적인 과정은 아래와 같다.
							고주파만 생성한다.  ->  [-128, 127] 범위로 정규화한다.  ->  편향값으로 +128을 더한다.
							곱/나눔 연산 후에 합/차 연산을 하듯이, 편향값 적용은 가장 마지막에(함수 내부에서 반환 직전에) 한다.
							embossing에서 normalization은 이렇게 사용된다.




***	sharpening과 unsharp-masking/edge-detection의 차이점은, 바로 위의 글을 이해했다면 알 수 있다.
	모든 윤곽의 비율을 보존할 수 있느냐, 없느냐의 차이이다.

	sharpening은, 가중치의 합이 1인 kernel을 받은 이상, 영상의 밝기를 보존하기 위해서 절대로 normalizing을 해서는 안되므로,
	어쩔 수 없이 overflow가 일어날 만큼 강한 윤곽을 버려야 한다.
	물론 구현을 어렵게 해서 살릴 수는 있으나, 그럴 바에 가장 정확한 unsharp-masking 혹은 edge-detection을 사용하는 게 훨씬 빠르다.

	unsharp masking은, LPF를 거치므로 overflow가 일어나지 않아서 모든 윤곽의 비율을 보존할 수 있다.
	edge-detection은, 고주파만 남으니 위의 글처럼 filter마다 normalizing 방법이 있다.
			





	< LPF > (Low-Pass Filter, 저주파 필터)  :  Smoothing(스무딩)  ->  Blurring(블러링), Noise Reduction(잡음 감소)

		LPF 중 linear filter에서는 mask에 적용되는 kernel의 모든 원소(mask의 모든 계수)가 양수이다.


			ㅡ Mean Filter (평균값 필터)
				Linear Spatial Filtering(선형 공간 필터링)이며, mask에 동일하게 가중치가 적용된다.
											1  1  1
				kernel_mean[3*3]  =	 1/9 *	1  1  1
											1  1  1



			ㅡ Gaussian Filter (가우시안 필터)
				// 참고 : https://sungw.tistory.com/24

				Linear Spatial Filtering(선형 공간 필터링)이며, mask의 중앙일수록 높은 가중치가 적용된다.

				가우시안 분포(Gaussian distribution) 함수를 근사하여 생성한 필터 마스크를 사용하는 필터링이다.
				가우시안 분포 혹은 정규 분포(normal distibution)는, 평균을 중심으로 좌우 대칭의 종 모양을 갖는 확률 분포를 말한다.

				// 픽셀 값이 급격하게 변경되는 에지 근방에 동일한 가우시안 필터가 적용되면 잡음뿐만 아니라 에지 성분까지 함께 감소하게 된다.
				// 즉, 잡음이 줄어들면서 함께 에지도 무뎌지기 때문에 객체의 윤곽이 흐릿하게 바뀐다.
				// 이러한 단점을 보완하기 위해 많은 사람들이 에지 정보는 그대로 유지하면서 잡음만 제거하는 에지 보전 잡음 제거 필터(edge-preserving noise removal filter)에 대해 연구했고,
				// 이로 인해 bilateral filter(양방향 필터)가 생겨났으며, 양방향 필터는 에지 성분은 보다 더 유지하면서 가우시안 잡음을 효과적으로 제거하는 알고리즘이다.



			ㅡ Bilateral Filter (양방향 필터)

				Nonlinear Spatial Filtering(비선형 공간 필터링)이다.

				// gaussian noise(가우시안 잡음) 제거에 탁월하다.



			ㅡ Order-Statistic Filter (순서-통계 필터)
				// 참고 : https://m.blog.naver.com/PostView.naver?blogId=kss9409&logNo=221525830164&navType=by
				// 참고 : https://everyday-image-processing.tistory.com/170

				Nonlinear Spatial Filtering(비선형 공간 필터링)이다.
				즉, mask가 kernel을 거치지 않으며, kernel을 통한 weight(가중치)에 따른 convolution연산이 없다.
				대신, 중간값 선택을 위한 mask의 정렬이 필요하다.

				정렬된 mask에서 어떠한 알고리즘을 적용하느냐에 따라 여러 가지의 filter로 나뉜다.


					ㅡ Median Filter (중간값 필터)  :  Noise Removal(잡음 제거)

						정렬된 mask에서 중간값을 선택한다.

						salt-and-pepper noise(점잡음/소금&후추잡음) 제거에 탁월하다.

						mask의 형태가 다양하며, 다이아몬드형이 가장 많이 쓰이고, 블록형이 가장 효율이 좋다.

						median filter의 mask형태와 run-time 단축을 위한 개선된 알고리즘들이 존재한다.

							ㅡ hybrid median filter : median filter의 직선, 코너의 뭉특해짐을 보완한 알고리즘이다.
							ㅡ separable median filter : ??
							등



					ㅡ Max Filter (최댓값 필터)

						정렬된 mask에서 최댓값을 선택한다.
						// 구현 : mask와 정렬 없이 삼항연산자로 가능하다.



					ㅡ Min Filter (최솟값 필터)

						정렬된 mask에서 최솟값을 선택한다.
						// 구현 : mask와 정렬 없이 삼항연산자로 가능하다.



					ㅡ Mid-point Filter (중간점 필터)

						정렬된 mask에서 최댓값과 최솟값의 평균을 선택한다.
						// 구현 : mask와 정렬 없이 삼항연산자로 가능하다.



					ㅡ α-trimmed Mean Filter (α-절삭 평균값 필터)
						// 참고 : https://m.blog.naver.com/dic1224/80182190750
						// 참고 : https://m.blog.naver.com/PostView.naver?isHttpsRedirect=true&blogId=dic1224&logNo=80180971021


						mean filter와 median filter의 장점을 갖춘 알고리즘이다.
						α(MIN) == mean filter
						α(MAX) == median filter

						α에 허용된 값 : { 0.1, 0.2, 0.3, 0.4, 0.5 }

						실수 α 값에 따라 정렬된 mask의 양쪽에서 동일한 개수를 버린다.


							// n : mask 행렬 원소의 개수. ( mask[3*3]에서 n = 9 )
								int TEMP = (int)(n * α);
								for (int i = TEMP; i < n - TEMP; ++i)
								{
									int SUM += mask[i];
								}
								return SUM / (n - 2 * TEMP);

								N_remainder = n - 2 * TEMP;

								output[width * y + x] = SUM / N_remainder;


							// n = 9(3by3)에서 α 값에 따른 TEMP값과, 평균값을 구하기 위한 남은 원소의 개수.
								α(0.0)  ->  TEMP(0), N_remainder(9)		XXXXXXXXXXXX
								α(0.1)  ->  TEMP(0), N_remainder(9)  == mean filter
								α(0.2)  ->  TEMP(1), N_remainder(7)
								α(0.3)  ->  TEMP(2), N_remainder(5)
								α(0.4)  ->  TEMP(3), N_remainder(3)
								α(0.5)  ->  TEMP(4), N_remainder(1)  == median filter
								α(0.6)  ->  TEMP(5), N_remainder(-1)?	XXXXXXXXXXXX







	< HPF > (High-Pass Filter, 고주파 필터),    < Edge Filter > (edge detection/Enhancement filter) (윤곽선 검출/강화 필터)
		// 참고 : https://m.blog.naver.com/PostView.naver?blogId=kss9409&logNo=221538477716&navType=by


			ㅡ Sharpening (샤프닝, 첨예화)

				영상의 밝기를 유지함과 동시에 edge를 더 강조하는 기법이다. (가중치의 합 = 1)

				noise 또한 edge가 될 수 있으며, 이를 방지하기 위해서 일반적으로 smoothing(noise-reduction)을 전처리 과정으로 사용한다.
				이러한 전처리 과정을 unsharpening, 출력 영상을 대충 unsharpened image라 할 수 있다.

				// 일반적인 HPF에 비해 저주파 영역의 상쇄에 해당하는 부분에 일정량의 이득을 줌으로써
				// HPF에서 발생하는 낮은 공간 주파수의 성분이 손실되는 문제점을 보완해주는 마스크이다.
				// sharpened영상은 origin에 origin_HPF(edge)를 합한 것과 비슷한 결과를 얻는다.

				sharpening은 자기 자신이 돋보여야 하므로, 마스크를 주변의 값들과 대비되도록 만든다.
				따라서 원래의 화소 값을 n배, 주변의 화소 값을 -1배 하여 합이 1이 되도록 마스크를 만든다.
				kernel의 중앙값이 클수록 원본 영상보다 sharpening 효과가 크다.
											+0  -1  +0			+1  -2  +1			-1  -1  -1			+0  -2  +0
				kernel_sharpening[3*3]  =	-1  +5  -1	  or	-2  +5  -2	  or	-1  +9  -1	  or	-2  +9  -2	  or	...
											+0  -1  +0			+1  -2  +1			-1  -1  -1			+0  -2  +0



			ㅡ 아래의 식은 굉장히 유용하게 사용될 수 있다.
				edge = (origin) - (origin_smoothing)		ㅡ [ㄱ]
				or
				edge = (origin_sharpening) - (origin)		ㅡ [ㄴ]


					ㅡ Unsharp Masking (언샤프 마스킹), Highboost Filtering (하이부스트 필터링)
						// 참고 : https://everyday-image-processing.tistory.com/141

						[ㄱ]의 방법이 사용되며, HPF의 미분을 통한 구현을 사용하지 않고 edge를 찾거나 강조할 수 있다는 장점이 있다.


						-- 아래와 같은 식으로 edge를 구한다.
						(edge) = (origin) - (origin_smoothing)

						-- 원본 영상에 edge를 얼마나 더할 것인지에 대한 계수 k(k >= 0)를 생성하여 식을 쓰면,
						(origin_sharpening) = (origin) + ( k * (edge) )

						-- 이 과정을 통해 생성한 결과를 unsharp mask(언샤프 마스크)라 하며,
						-- (k = 1)이면 unsharp masking이고,
						-- (k > 1)이면 highboost filtering이다.



			ㅡ Isotropic Filter (등방성 필터)

					ㅡ Laplacian Filter (라플라시안 필터)
						// 참고 : https://everyday-image-processing.tistory.com/140

						laplacian을 통해 생성한 kernel을 사용하는 filter이다.
						// 수학에서 라플라스 연산자(Laplace operator)/라플라시안(Laplacian)은 2차 미분 연산자의 일종으로, 기울기의 발산이다.
													+0  +1  +0			+1  +1  +1			+0  -1  +0			-1  -1  -1			-1  +2  -1
						kernel_laplacian[3*3]  =	+1  -4  +1	  or	+1  -8  +1	  or	-1  +4  -1	  or	-1  +8  -1	  or	+2  -4  +2	  or	...
													+0  +1  +0			+1  +1  +1			+0  -1  +0			-1  -1  -1			-1  +2  -1

						laplacian filter를 거친 영상을 g(x,y)라 하면,

						g(x,y) = f(x,y) + c[Δf(x,y)]    ( c = +1 or -1 )

						복잡한 과정을 생략하고 kernel의 순서대로 (a), (b), (c), (d)라 할 때,
						kernel의 중앙값이 음수인 (a), (b)에 대해서는 [ c = 1 ] 이며,
						kernel의 중앙값이 양수인 (c), (d)에 대해서는 [ c = -1 ] 이다.



			ㅡ Anisotropic Filter (비등방성 필터)

					ㅡ Embossing (엠보싱)
						// 참고 : https://psh7286.tistory.com/entry/OpenCV-4-7-%ED%95%84%ED%84%B0%EB%A7%81

						영상의 모든 pixel값을 높이로 하는 3D모형에서,
						{
							correlation : kernel의 [-1 -> +1] 방향으로 햇빛을 쬔다.
							convolution : kernel의 [+1 -> -1] 방향으로 햇빛을 쬔다.
						}

						embossing의 전체적인 과정은 다음과 같다.
						고주파만 생성한다.  ->  [-128, 127] 범위로 정규화한다.  ->  편향값으로 +128을 더한다.

						// 결국에 가장 강한 상승고주파가 255의 값을 가지고, 가장 강한 하강고주파가 0의 값을 가지게 된다.

													-1  -1  +0			-1  +0  +0			+0  +1  +0
						kernel_embossing[3*3]  =	-1  +0  +1	  or	+0  +0  +0	  or	+0  +0  +0	  or	...
													+0  +1  +1			+0  +0  +1			+0  -1  +0

						// 위와 같이 빛의 범위를 두 종류로, 빛의 방향을 다르게 하여 16가지가 있다.

						// 영상의 경계선 방향이 빛의 방향과 일치할수록, 경계선이 검출되지 않는다.
						// 빛의 방향에 따라 kernel의 형태도 다양하다.

						// kernel을 변형, 햇빛을 여러 방향에서 쬐어 생성한 값들을 합성하여 다방면의 윤곽을 검출하거나,
						// 회색값으로 편향하지 않고 edge-detection으로 사용하는 것은, 추천하지 않는다.
						// 각각의 filter는 정해진 목적이 존재하며,
						// embossing filter는, "엠보싱"이라는 디자인을 보여주는 filter의 한 종류로서 구현되어야 한다.
						// 위와 같이 변형된 filter는 customized filter로 따로 분류해야 한다.



					ㅡ Roberts cross-gradient operator (로버츠 교차-그래디언트 연산자), Sobel operator (소벨 연산자)
						// 참고 : https://everyday-image-processing.tistory.com/141

						gradient를 통해 생성한 kernel을 사용하는 filter이다.
						// 수학에서 그래디언트는, 모든 다변수/다방면 함수에 대한 1차 미분을 말한다.

						[ 참고글에 있는 수식들 ]

						각각 두 개의 kernel을 적용한 결과를 위의 수식에 대입하는 것으로 edge_detection이 수행된다.

						// 수식을 모르기 때문에 kernel을 알아도 의미가 없다. kernel이 궁금하면 검색하면 나온다.







	< Histogram Modification > (히스토그램 변형)  ==  Histogram Matching (히스토그램 매칭)  ==  Histogram Specification (히스토그램 명세화)

		입력 영상의 histogram을 원하는 histogram으로 변환하는 기법이다.
		// 영상의 brightness와 contrast를 개선할 수 있다.

		영상 전체에서 수행하는 global processing과, (전역 처리)
		영상 일부분에서 수행하는 local processing이 있다. (지역 처리)

			// 전역 처리 기법은 영상의 전반적인 향상에는 적합하나, 영상 내의 상세한 정보는 개선시키지 못한다는 단점이 있다.
			// 이를 위해서 히스토그램 평활화나 히스토그램 지정을 전체 영상에 적용하는 것이 아니라,
			// 중앙 화소를 기점으로 하는 영상보다 작은 window에서 처리하고 한 픽셀 이동 후 해당 window에서 처리하는 방법인 지역 처리 기법이 있다.

			// 이때 window size는 3*3, 5*5와 같은 작은 크기로 하며,
			// 다양한 window size 중에서 출력 영상의 품질을 보고 최종 window를 선택한다.

			// 지역 처리 기법의 히스토그램 변형은, 공간 필터링과 내부 연산이 다른 것 빼고 유사하다.


					continuous variable(연속 변수), discrete variable(이산 변수)

					probability function(확률 함수)
						확률 변수가 취할 수 있는 결과에 확률을 할당하는 함수를 의미하는 일반적인 용어이다.
						continuous/discrete variable 모두 포괄한다.

					probability distribution(확률 분포)
						확률 변수가 가질 수 있는 값들에 대한 확률이 어떻게 분포되어 있는지를 설명하는 넓은 개념이다.
						확률 변수의 동작에 대한 완전한 설명으로, 모든 가능한 결과와 그에 따른 확률을 포함한다.
						continuous/discrete variable 모두 포괄한다.

					CRV (Continuous Random Variable, 연속확률변수)
					PDF (Probability Density Function, 확률밀도함수)
					CDF (Cumulative Distribution Function, 누적분포함수) : PDF와 미적분 관계이다.

						PDF(probability density function, 확률밀도함수)와 PDF(probability distribution function, 확률분포함수)의 차이에 대해서
						-> https://velog.io/@groovallstar/%ED%99%95%EB%A5%A0-%EB%B6%84%ED%8F%AC-%ED%95%A8%EC%88%98%EC%99%80-%ED%99%95%EB%A5%A0-%EB%B0%80%EB%8F%84-%ED%95%A8%EC%88%98%EC%9D%98-%EC%9D%98%EB%AF%B8

					DRV (Discrete Randon Variable, 이산확률변수)
					PMF (Probability Mass Function, 확률질량함수)
					CMF (Cumulative Mass Function, 누적질량함수) : PMF와 누적/차분 관계이다.


		LUT (Loop-Up Table, 룩업테이블) : histogram 변형을 위한 특정 LUT를 생성하고, 영상이 LUT를 거쳐서 변형된 영상을 얻는다.

		// 연산 과정에서 CRV를 따르는 PDF, CDF가 생성될 수 있으나,
		// 결과적으로 digital image와 histogram은 DRV를 따르며, histogram과 LUT는 각각 PMF, CMF를 의미한다.



			ㅡ Histogram Scaling (Stretching)

				해당 영역의 범위를 늘린다.



			ㅡ Histogram Scaling (Shrinking)

				해당 영역의 범위를 줄인다.



			ㅡ Histogram Sliding

				범위를 shift한다.



			ㅡ Histogram Matching (히스토그램 지정)
				// 참고 : https://everyday-image-processing.tistory.com/134

					// 하나도 이해가 안된다.



			ㅡ Histogram Equalization(균일화, 균등화, 평활화, spreading)

				분포 모양을 균일하게 변형시킴으로써 픽셀 값의 범위를 고르게 한다.
				이상적으로 PMF는 평평하게(flatter), CMF는 균일한 증가량을 가지게 된다.
				일반적으로 영상의 CMF를 LUT로 사용하는 방법이 있다. ( -> 교수님이 내주신 histogram equalization의 첫 과제가 이거다.)

				// 부작용 해결방법에 대한 논문 : https://scienceon.kisti.re.kr/commons/util/originalView.do?cn=JAKO201015233599145&oCn=JAKO201015233599145&dbt=JAKO&journal=NJOU00294921



					ㅡ equalization에 주로 사용되는 LUT(Look-Up Table)에 대하여.

							영상이, 영상의 LUT를 거쳐서 equalized영상을 얻는다.
							LUT를 어떠한 방식으로 만드는지, 어떠한 형태인지에 따라서 영상의 equalizing 결과가 다르다.

							grayscale에 적용할 LUT의 형태를 구하는 과정을 생각해보자.

							input[j][i] -> x		// 입력 영상의 특정 화소값.
							output[j][i] -> y		// 출력 영상의 특정 화소값.
							LUT -> (function) F

							영상이 LUT를 거쳐서 EQ영상이 생성되는 것을 수식으로 나타내면,
							"y = F(x)" 이다.

									e.g.	영상이 LUT를 거쳐서 동일하게 나올 때 LUT의 그래프는 "y = x" 이다.

							함수(LUT)는 input(x축)과 output(y축)이 있어야 한다.
							1차원 배열의 경우,
							각각의 index 번호가 x축이고 해당 index번호의 값이 y축이므로,
							영상에 대해서 1차원 배열을 LUT로 사용할 수 있다.

							정리하여 코드로 나타내면,

							"output[j][i] = LUT[input[j][i]];"


							"10주차_과제_2"에서의 LUT는 미리 만들어 둔 LUT가 아닌, input image를 통해 생성한 LUT였다.
							여러 가지 LUT그래프를 만들어 둔다면 그 자체로 사진편집어플의 필터기능일 것이다.







	< image binarization > (영상 이진화 + thresholding(임계값 설정))  ==  image segmentation (영상 분할)

		영상처리에서 이진화란, 어떤 주어진 임계값(threshold, T)보다 밝은 픽셀들은 모두 MAX로, 그렇지 않은 픽셀들은 모두 MIN으로 바꾸는 것을 말한다.

		// 다중경계의 입체적 표현에서는 이진화 구간에 따라 [MAX, MIN]값을 다르게 한다.
		// 레터링 추출과 같은 단일경계에서는 [MAX, MIN]값을 하나만 가진다.
		// 일반적으로 이진화한 영상을 monochrome으로 표현하며, 우리가 실습으로 사용하는 흑색조 = grayscale은 monochrome에 속한다.

		// thresholding = 임계값 설정, 임계처리, 임계값 처리

		임계값을 구하기 위해서, 영상의 histogram을 활용할 수 있다.

		"전역 고정 이진화", "지역 가변 이진화"로 나뉜다.







	< Global Fixed Thresholding > (전역 고정 이진화)

		하나의 임계값만을 사용하여 이진화하는 방법이다.
		그 임계값을 어떻게 자동으로 결정하느냐가 중요하다.

		optimal threshold (optimal T) : 이진화에 따른 분류 에러(classification error)를 최소화하는 임계값이다.

			ㅡ Lab의 flower영상  or  흰 바탕에 검은 글씨
			위의 영상들의 histogram은 거의 완벽하게 배경 영역과 물체 영역의 두 클래스로 나뉘므로,
			임계값 T를 구하는 것은 아주 쉽다.

			ㅡ Otsu's method (오츠의 알고리즘) : 최적의 임계값을 찾는 유명한 알고리즘이다.
			임계값 T를 기준으로 영상 픽셀들을 두 클래스로 분류했을 때,
			두 클래스간의 intra-class variance를 최소화하거나 또는 inter-class variance를 최대화하는 T를 찾는 이진화 방법이다.

			ㅡ google search -> "adaptive thresholding" -> image.
			ㅡ Lab의 lena영상.
			문제에 따라서는 위의 영상들과 같이 하나의 임계값(threshold)만으로는 어떤 값을 주어도 정확한 이진화가 힘든 경우가 있다.







	< Local Adaptive Thresholding > (지역 가변 이진화)
		// 참고 : https://darkpgmr.tistory.com/115
		// 참고 : https://sungw.tistory.com/25?category=926149

		픽셀 위치마다 서로 다른 임계값을 사용하는 가변 thresholding 기법을 통한 이진화 방법이다.
		Pixel Area(Group) Processing(화소 영역 처리, 픽셀 그룹 처리)에 속한다.
*/

/*
ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ
ㅡ 미처리, 미분류
ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ

	1.
	종훈이랑 내가 했던, 변화율의 고정값 차를 이용한 이진화.
	2.
	1의 방법에서 고정값이 아닌 비율 차(%).

	>> 1번, 2번 모두 mask의 2D 이산미분그래프를 활용한 "지역 가변 이진화"이다.





	openCV : 실시간 컴퓨터 비전을 목적으로 한 프로그래밍 라이브러리이다. C/C++에 있는진 모르겠는데, 유명하니까 알아두자.





	구글링 결과, 대부분의 글이 블러링과 스무딩을 동일시한다.
	실제로 openCV에서도
	또한 블러링과 스무딩의 차이를 찾는데 사실상 실패했으며, 단 두 개를 찾았는데 이를 여기에 적었다.

		참고 : https://marisara.tistory.com/entry/%ED%8C%8C%EC%9D%B4%EC%8D%AC-openCV-13-%EB%B8%94%EB%9F%AC%EB%A7%81blurring%EC%8A%A4%EB%AC%B4%EB%94%A9smoothing%EC%83%A4%ED%94%84%EB%8B%9Dsharpening

				ㅡ Smoothing (스무딩)
				스무딩은 블러링과 비슷하게 주변 화소들의 평균으로 적용하지만,
				주변 화소들에 편차를 주어 계산한다.


				e.g. kernel_smoothing[3*3]			1  2  1
											1/16 *	2  4  2
													1  2  1

					// 정리하자면,
					// blurring : 동일한 가중치의 mask, 동일한 계수의 kernel을 사용한 filter를 말한다.
					// smoothing : blurring 이외에 가중치/계수가 다른 것들을 말한다.

					// 이에 따르면,
					// 평균값 필터는 mean-blurring이 되고, 가우시안 필터는 Gaussian-smoothing이 된다.
					// 그런데 또 openCV에서 가우시안 필터의 이름이 GaussianBlur()이다.??

					// 위의 두 filter를 "-blurring" 또는 "-smoothing"으로 구글링을 통한 교차 비교 결과,
					// 한국, 영어권 모두 구별이 가지 않으므로 나 또한 동일시한다.



		참고 : https://webnautes.tistory.com/1044
		참고 : https://www.geeksforgeeks.org/spatial-filtering-and-its-types/

				smoothing : 영상을 흐리게(blurring) 하거나, 영상에 존재하는 노이즈를 제거(noise-reduction)하기 위해 사용된다고 한다.

					// 여기서는 smoothing이 blurring과 noise-reduction을 포함한다.



	결과 : 사이트를 3개로 분류해본다.

		1. 처음 사이트.

		2. 두 번째 사이트.

		3. 용어를 정의할 때 smoothing과 blurring이 같다고 한다.
			이러한 용어 정의를 했던 안 했던 이후의 내용은
			smoothing 혹은 blurring 중 하나로 통일이 되어 있다.


		1번은 저 하나 뿐이고, 3번이 90% 이상?
		2번은 비중이 없진 않다.
		나도 2번으로 쓰되, 정의하거나 강조하지는 말자.





	참고 : https://sijoo.tistory.com/40

		noise model (잡음 모델) : 잡음이 생성되는 방식에 따라 분류된다.

			ㅡ gaussian noise(가우시안 잡음) : 정규 분포를 갖는 noise이며, 대표적인 잡음이다.
			ㅡ Rayleigh(레이)
			ㅡ Erlang/Gamma(감마)@@@@@
			ㅡ Exponential(지수)
			ㅡ Uniform(균일)
			ㅡ Impulse(임펄스) : salt-and-pepper noise(소금&후추 잡음, 점 잡음)





	참고 : https://eehoeskrap.tistory.com/99

		필터의 종류  ->  유한 임펄스 응답 필터, 무한 임펄스 응답 필터.





	이 글에서는 mask, kernel, window, template 각 용어의 개념을 확실하게 분리하여 사용했다.





	median filter는 nonlinear-filter로서 noise의 완벽 제거가 가능하다.
	linear-filter로는 noise의 완벽 제거가 힘들다. 우리 눈에 안보이면 땡이긴 하지만.
	그리고 noise reduction과 blurring을 동시에 작업하기 위한 median filter는, 다른 LPF들과 비교하여 상당히 좋다.
	잡음제거 성능은 물론이고, 에지 흐려짐 정도도 가장 작고, 여튼 아주 좋다.
	오히려 median filter의 단점/부작용이 있을 테니, 이를 찾아보자.





	참고 : https://velog.io/@lighthouse97/%EC%98%81%EC%83%81Image%EC%9D%98-%ED%95%A9%EC%84%B1%EA%B3%B1Convolution%EC%9D%98-%EC%9D%B4%ED%95%B4

		CNN(convolution neural network)에서의 convolution연산의 과정을 자세히 설명한다. // AI분야에 가깝다.
		또한 지금 수준에 사용 가능한 용어/개념이 몇 개 있다.

			Stride(스트라이드, 보폭)
				필터링은 필터를 이미지에 놓고 움직이면서 한다.
				이때 필터가 한번에 움직이는 거리를 보폭, 흔히 스트라이드(stride)라고 부른다.
				커널이 한 칸씩 움직이면 스트라이드 값은 1이 되고, 두 칸씩 움직이면 2가 된다.

			feature map(특성 지도)

			shared weight(가중치 공유)

			padding(패딩)

			activation function(활성 함수)

			ReLU(rectified linear unit)

			pooling(풀링) - max-pooling, average-pooling, stochastic-pooling

			overfitting(오버피팅)

			channel(채널)

			recognition(인식)





	참고 : https://m.blog.naver.com/PostView.naver?blogId=kss9409&logNo=221515037321&navType=by

		Ends-in 탐색 기법
		threshold 적용 방법 3가지





	참고 : https://m.blog.naver.com/PostView.naver?blogId=kss9409&logNo=221525830164&navType=by

		spatial processing(공간 처리)  ==  neighborhood processing

		average filter(평균 필터)  ->  box filter(상자 필터, 균일한 가중치), weighted average filter(가중치 평균 필터)

			box filter(상자 필터) : mena filter(평균값 필터)
			weighted average filter(가중치 평균 필터) : gaussian filter(가우시안 필터)

		잡음 확률 밀도 함수의 형태
			1. 가우스 분포(Gaussian Noise) : 실제 발생하는 잡음은 대부분 가우스 분포를 따른다.
			2. 단일 분포(Uniform Noise)
			3. 임펄스 분포(Impulse Noise)





	디지털 영상 처리는 "문제 지향적(problem-oriented)"이기 때문에, 특정 문제를 풀기 위해서 다양한 방법을 적용할 수 있다.





	참고 : https://everyday-image-processing.tistory.com/137

		linear filter에 사용되는 연산으로 spatial correlation(공간 코릴레이션)과, spatial convolution(합성곱)이 있다.
		이 둘은 SOP 형식이라는 점이 동일하지만,
		convolution은, correlation을 적용하기 전에 mask가 180도 회전한다.

		// 그러면 correlation 자체로는 SOP(sum of products)와 의미가 같지 않을까 싶다.

		// 그런데, linear filter의 정의에 따라서 convolution의 회전이 의미가 없는 것 아닌가??? ( + 의미 없는 것이 맞다. )
		// 실제로 이수진 교수님도 correlation에 대한 언급은 없었으며, convolution이라 적어놓고 회전 또한 언급이 없다.
		// 일단은 convolution이란 용어로 쓰되, 나중에 filter 구현할 때 찾아보자.





	smoothing spatial filter(스무딩 공간 필터)  ->  smoothing linear filter(스무딩 선형 필터), order-statistic filter(순서-통계 필터)





	2차원 행렬끼리의 연산에서는 이중for문이 필요한데,
	이를 한 번에 처리하도록 vector로 변환하여 연산할 수 있다.





	참고 : https://everyday-image-processing.tistory.com/?page=35

		주파수 공간 필터링은 여기서 이어서 하면 된다.
		많은 선행 지식을 필요로 하기 때문에, 나중에 프로젝트에 필요하면 배우자.





	참고 : https://www.mathworks.com/help/images/what-is-image-filtering-in-the-spatial-domain_ko_KR.html

		선형 필터링이란 출력 픽셀의 값이 입력 픽셀의 이웃 픽셀들의 값의 일차 결합이 되는 필터링입니다.




ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ
ㅡ convolution의 180도 회전 이유에 대해서 찾아보자.
ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ


	참고 : https://wiserloner.tistory.com/845
	"영상 처리 분야에서는 대부분 필터가 대칭형태의 정방형이기에, 필터링 연산을 컨볼루션 연산이라고 부르고 있습니다."


		이거, convolution 설명에 확실하게 적어두자.
		용어는 convolution이라 하되, 일단 구현은 correlation으로 한다고.
		embossing에도 이 두 개로 나눠서 빛방향 설명해두자.

			[코릴레이션]
			- 필터링 연산 방식은 알겠죠?
			필터 내 원소들과 동위의 영상 픽셀 값을 곱해준 후, 그 값을 모두 더해주는 연산이란,
			말하자면 두 영상 신호간의 '유사성'을 판단하는 연산이라 할수 있습니다.
			이런 것을 신호 처리분야에서는 Correlation 또는 상관이라고 합니다.

			[컨볼루션]
			- 필터링 연산을 코릴레이션이라고도 하는데,
			다른 말로는 컨볼루션이라고도 합니다.
			- 컨볼루션Convolution은 합성곱으로,
			코릴레이션과 계산법이 동일한데, 조금더 제한을 두자면,
			필터가 중심점을 기준으로 대칭되게 이루어진 필터를 사용하는 것입니다.
			- 영상 처리 분야에서는 대부분 필터가 대칭형태의 정방형이기에, 필터링 연산을 컨볼루션 연산이라고 부르고 있습니다.


		이거, "180도 법칙"같은데?? https://m.blog.naver.com/blockbuster3/220597830367
		180도 법칙이란 편집 규칙의 일종이다.
		그러나 더 정확히 말하면 편집에서 지켜야 하는 규칙이라기 보다는 ‘올바른 편집을 위해 촬영시에 지켜야 하는’ 규칙1이다.


	참고 : http://contents.kocw.or.kr/document/lec/2012/Sangmyung/YooHyeonJoong/05_1.pdf

		convolution(합성곱)의 sifting(시프팅) 특성.

		sifting : 단순한 복사 특성.
					// 선형 시스템 이론의 근원적 개념이다.
					// convolution에서 180도 회전하는 이유.



*/