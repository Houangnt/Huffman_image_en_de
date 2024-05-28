#include "imageIOException.h"


ImageIOException::ImageIOException(const string &__arg) :
        runtime_error("ImageIOException: " +  __arg) {}
