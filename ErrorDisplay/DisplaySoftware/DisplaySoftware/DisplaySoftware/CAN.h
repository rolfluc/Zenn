#pragma once

typedef void(*cancb)();
void InitCAN(cancb cb);