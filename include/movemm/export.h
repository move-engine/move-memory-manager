#pragma once

#ifndef MOVEMM_EXPORT
#if defined(MOVE_WINDOWS) && !defined(MOVEMM_STATIC)
#if defined(MOVEMM_EXPORTING)
#define MOVEMM_EXPORT extern "C" __declspec(dllexport)
#else
#define MOVEMM_EXPORT extern "C" __declspec(dllimport)
#endif
#else
#define MOVEMM_EXPORT
#endif
#endif