#pragma once

#ifndef __cplusplus
extern "C" {
#endif

int create3_connect();
int create3_connect_manual(const char *const host, const unsigned short port);

int create3_is_connected();

void create3_set_velocity(const double linear_x, const double angular_z);

#ifndef __cplusplus
}
#endif