/** 
 @file  types.h
 @brief type definitions for ENet
*/
#ifndef __ENET_TYPES_H__
#define __ENET_TYPES_H__

#ifdef WIN32
  typedef unsigned char enet_uint8;
  typedef unsigned short enet_uint16;
  typedef unsigned int enet_uint32;
#else
  #include <inttypes.h>
  typedef uint8_t enet_uint8;
  typedef uint16_t enet_uint16;
  typedef uint32_t enet_uint32;
#endif

#endif /* __ENET_TYPES_H__ */
