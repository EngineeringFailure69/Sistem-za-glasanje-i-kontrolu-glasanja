#pragma once
#define _CRT_SECURE_NO_WARNINGS
#define WIN32_LEAN_AND_MEAN

#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "27015"
#define SERVER_ADDRESS "127.0.0.1"

#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>
#include<stdbool.h>
#include<string.h>
#include<Windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include<curl/curl.h>
#include"DrawFunkcije.h"
#include"Utils.h"
#include "Strukture.h"
#include "KreiranjeNaloga.h"
#include "Ekrani.h"