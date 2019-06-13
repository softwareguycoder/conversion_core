// base64.c - Implementations of the functionality to Base64-encode and decode
// data
//
// Shout-out to the blog at:
// <https://nachtimwald.com/2017/11/18/base64-encode-and-decode-in-c/>
//

#include "stdafx.h"
#include "conversion_core.h"

#include "base64.h"

///////////////////////////////////////////////////////////////////////////////
// File-scope globals

const char b64chars[] =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

///////////////////////////////////////////////////////////////////////////////
// Internal-use-only functions

///////////////////////////////////////////////////////////////////////////////
// GenerateBase64DecodeTable function

void GenerateBase64DecodeTable(int** ppnInverseMappingTable, int nSize)
{
  if (ppnInverseMappingTable == NULL) {
    return; // Required parameter
  }

  *ppnInverseMappingTable = (int*) malloc(nSize * sizeof(int));
  if (*ppnInverseMappingTable == NULL) {
    return; // Failed to allocate storage
  }

  int i = 0;
  for (i = 0; i < nSize; i++) {
    (*ppnInverseMappingTable)[i] = -1;
  }
  for (i = 0; i < sizeof(b64chars) - 1; i++) {
    char ch = b64chars[i];
    int nIndex = ((int) ch) - 43;
    (*ppnInverseMappingTable)[nIndex] = i;
  }
}

///////////////////////////////////////////////////////////////////////////////
// IsValidBase64Char function

BOOL IsValidBase64Char(char ch)
{
  if (ch >= '0' && ch <= '9')
    return TRUE;
  if (ch >= 'A' && ch <= 'Z')
    return TRUE;
  if (ch >= 'a' && ch <= 'z')
    return TRUE;
  if (ch == '+' || ch == '/' || ch == '=')
    return TRUE;
  return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
// Publicly-exposed functions

///////////////////////////////////////////////////////////////////////////////
// Base64Decode function

BOOL Base64Decode(const char *pszEncodedBytes,
    unsigned char *pszDecodedBytes, int nOutputSize)
{
  int nEncodedBytes = 0;
  int nCurVal = 0;

  if (pszEncodedBytes == NULL || pszDecodedBytes == NULL)
    return FALSE;

  if (nOutputSize <= 0) {
    return FALSE;
  }

  nEncodedBytes = strlen(pszEncodedBytes);
  if (nOutputSize < GetBase64DecodedDataSize(pszEncodedBytes)
      || nEncodedBytes % 4 != 0)
    return FALSE;

  for (int i = 0; i < nEncodedBytes; i++) {
    if (!IsValidBase64Char(pszEncodedBytes[i])) {
      return FALSE;
    }
  }

  int* b64invs = NULL;
  GenerateBase64DecodeTable(&b64invs, 80);

  for (int i = 0, j = 0; i < nEncodedBytes; i += 4, j += 3) {
    nCurVal = b64invs[pszEncodedBytes[i] - 43];
    nCurVal = (nCurVal << 6) | b64invs[pszEncodedBytes[i + 1] - 43];
    nCurVal =
        pszEncodedBytes[i + 2] == '=' ?
            nCurVal << 6 :
            (nCurVal << 6) | b64invs[pszEncodedBytes[i + 2] - 43];
    nCurVal =
        pszEncodedBytes[i + 3] == '=' ?
            nCurVal << 6 :
            (nCurVal << 6) | b64invs[pszEncodedBytes[i + 3] - 43];

    pszDecodedBytes[j] = (nCurVal >> 16) & 0xFF;
    if (pszEncodedBytes[i + 2] != '=')
      pszDecodedBytes[j + 1] = (nCurVal >> 8) & 0xFF;
    if (pszEncodedBytes[i + 3] != '=')
      pszDecodedBytes[j + 2] = nCurVal & 0xFF;
  }

  free(b64invs);

  return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
// Base64Encode function

void Base64Encode(const unsigned char *pszBinaryInput, int nInputLength,
    char** ppszOutput, int *pnOutputLength)
{
  if (pszBinaryInput == NULL || nInputLength <= 0) {
    return;
  }

  if (ppszOutput == NULL) {
    return; // Required parameter
  }

  int nOutputLength = GetBase64EncodedDataSize(nInputLength);
  *ppszOutput = (char*) malloc((nOutputLength + 1) * sizeof(char));
  memset(*ppszOutput, 0, nOutputLength + 1);

  (*ppszOutput)[nOutputLength] = '\0';

  for (int i = 0, j = 0; i < nInputLength; i += 3, j += 4) {
    int nValue = pszBinaryInput[i];
    nValue = i + 1 < nInputLength ?
                                    nValue << 8 | pszBinaryInput[i + 1]
                                        :
                                    nValue << 8;
    nValue =
        i + 2 < nInputLength ?
                               nValue << 8 | pszBinaryInput[i + 2]
                                   :
                               nValue << 8;

    (*ppszOutput)[j] = b64chars[(nValue >> 18) & 0x3F];
    (*ppszOutput)[j + 1] = b64chars[(nValue >> 12) & 0x3F];
    if (i + 1 < nInputLength) {
      (*ppszOutput)[j + 2] = b64chars[(nValue >> 6) & 0x3F];
    } else {
      (*ppszOutput)[j + 2] = '=';
    }
    if (i + 2 < nInputLength) {
      (*ppszOutput)[j + 3] = b64chars[nValue & 0x3F];
    } else {
      (*ppszOutput)[j + 3] = '=';
    }
  }

  *pnOutputLength = nOutputLength;

  return;
}

///////////////////////////////////////////////////////////////////////////////
// GetBase64DecodedDataSize function

int GetBase64DecodedDataSize(const char *pszInputText)
{
  int nInputLength = 0;
  int nResult = 0;

  if (pszInputText == NULL || pszInputText[0] == '\0') {
    return 0;
  }

  nInputLength = strlen(pszInputText);
  if (pszInputText[nInputLength - 1] == '\n') {
    nInputLength--;
  }
  nResult = nInputLength / 4 * 3;

  for (int i = nInputLength; i-- > 0;) {
    if (pszInputText[i] == '=') {
      nResult--;
    } else {
      break;
    }
  }

  return nResult;
}

///////////////////////////////////////////////////////////////////////////////
// GetBase64EncodedDataSize function

int GetBase64EncodedDataSize(int nInputLength)
{
  int nResult = 0;

  if (nInputLength <= 0) {
    return nResult;
  }

  nResult = nInputLength;
  if (nInputLength % 3 != 0) {
    nResult += 3 - (nInputLength % 3);
  }
  nResult /= 3;
  nResult *= 4;

  return nResult;
}

///////////////////////////////////////////////////////////////////////////////
// IsValidBase64EncodedString function

BOOL IsValidBase64EncodedString(const char* pszEncodedBytes,
    int nEncodedBytes) {
  // TODO: Add implementation code here
  return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
