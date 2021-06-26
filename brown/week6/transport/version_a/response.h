#pragma once

#include <memory>
using namespace std;
struct BaseResponse;

using ResponseHolder = unique_ptr<BaseResponse>;