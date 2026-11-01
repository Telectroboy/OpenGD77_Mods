/*
 * F4EGM: APRS channel label selection for the MDUV380 variant.
 * This code supplements, but does not replace, the upstream APRS encoder.
 * The original OpenGD77 project license remains applicable to the firmware.
 */
#ifndef F4EGM_APRS_CHANNEL_NAME_H
#define F4EGM_APRS_CHANNEL_NAME_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* The codeplug stores a fixed 16-byte channel name, not a C string.
 * 0xFF is the codeplug's unused-byte marker. VFO A/B MUST use the
 * original frequency-based APRS QSY information instead of a label.
 * Non-printable or extended characters fall back to the frequency:
 * APRS information fields are transmitted as ASCII text.
 */
static inline bool f4egmAprsExtractChannelName(const char rawName[16], bool isVFO, char label[17])
{
	char tmp[17];
	unsigned int len = 0U;
	unsigned int first = 0U;
	unsigned int last;

	if (label == NULL)
	{
		return false;
	}
	label[0] = '\0';
	if ((rawName == NULL) || isVFO)
	{
		return false;
	}

	for (unsigned int i = 0U; i < 16U; i++)
	{
		unsigned char c = (unsigned char)rawName[i];
		if ((c == 0U) || (c == 0xFFU))
		{
			break;
		}
		if ((c < 0x20U) || (c > 0x7EU))
		{
			return false;
		}
		tmp[len++] = (char)c;
	}

	last = len;
	while ((first < last) && (tmp[first] == ' '))
	{
		first++;
	}
	while ((last > first) && (tmp[last - 1U] == ' '))
	{
		last--;
	}
	if (last == first)
	{
		return false;
	}
	for (unsigned int i = first; i < last; i++)
	{
		label[i - first] = tmp[i];
	}
	label[last - first] = '\0';
	return true;
}

#endif /* F4EGM_APRS_CHANNEL_NAME_H */
