// Scanner tests

// @err: unterminated string constant
const char v1[] = "this is a string
;

const char v2[] = "\g"; // @err: bad escape character
const char v3[] = L"\u123"; // @err: bad universal character

// @err: unterminated string constant
const char v4[] = "this is a string
