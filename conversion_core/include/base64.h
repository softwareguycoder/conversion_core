// base64.h - Interface to Base64 encoding and decoding functionality
//

#ifndef __BASE64_H__
#define __BASE64_H__

#include <../../api_core/api_core/include/api_core.h>

/**
 * @name Base64Decode
 * @brief Decodes a string using the Base64 decoding.
 * @param pszEncodedBytes Address of a string of bytes that need decoding.
 * @param pszDecodedBytes Address of storage where the decoded bytes are
 * to be placed.
 * @param nOutputSize Size of the storage location that will hold the
 * decoded bytes.
 * @return Nonzero if the operation was successful.
 */
BOOL Base64Decode(const char *pszEncodedBytes,
    unsigned char *pszDecodedBytes, int nOutputSize);

/**
 * @name Base64Encode
 * @brief Encodes binary data in the Base-64 encoding scheme.  This scheme
 * enables the data to be transmitted over the network in a format understand-
 * able by systems without the capability to process binary data over the
 * network.
 * @param pszBinaryInput Address of storage holding the binary data to be
 * encoded.
 * @param nInputLength Length of the binary data, in bytes.
 * @param ppszOutput Address of storage that will receive the Base64-encoded
 * data.  free() must be called on this storage when the application is
 * done using it.
 * @param pnOutputLength Address of an integer variable that will receive the
 * length, in bytes, of the encoded data.
 */
void Base64Encode(const unsigned char *pszBinaryInput, int nInputLength,
    char** ppszOutput, int *pnOutputLength);

/**
 * @name GetBase64DecodedDataSize
 * @brief Calculates the minimum size for the storage (excluding, perhaps,
 * a terminating NULL character) needed to save the result of a Base64-Decode
 * operation.
 * @param pszInputText Encoded text.
 * @return Without actually performing the decode operation, calculates how
 * many bytes of storage are needed, at a minimum, were a decode operation
 * to be performed on the text pointed to by pszInputText.
 */
int GetBase64DecodedDataSize(const char *pszInputText);

/**
 * @name GetBase64DecodedDataSize
 * @brief Calculates the amount of storage needed for binary data after it's
 * been Base64-encoded, in bytes.
 * @param nInputLength Number of bytes of binary data that is needing to be
 * encoded.
 * @return Amount of storage, in bytes, needed for binary data after it's
 * been Base64-encoded.
 */
int GetBase64EncodedDataSize(int nInputLength);

/**
 * @name IsValidBase64EncodedString
 * @brief Determines whether the character buffer passed actually consists
 * of valid Base64-encoded text or not.
 * @param pszEncodedBytes Buffer of (signed) characters to check.
 * @return TRUE if the text in pszEncodedBytes is indeed Base64-encoded;
 * FALSE otherwise.
 */
BOOL IsValidBase64EncodedString(const char* pszEncodedBytes);


#endif //__BASE64_H__
