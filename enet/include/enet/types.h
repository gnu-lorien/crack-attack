/** 
 @file  types.h
 @brief type definitions for ENet
*/
#ifndef __ENET_TYPES_H__
#define __ENET_TYPES_H__

#ifdef WIN32
  typedef unsigned char enet_uint8;       /**< unsigned 8-bit type  */
  typedef unsigned short enet_uint16;     /**< unsigned 16-bit type */
  typedef unsigned int enet_uint32;      /**< unsigned 32-bit type */
#else
  #include <inttypes.h>
  typedef uint8_t enet_uint8;       /**< unsigned 8-bit type  */
  typedef uint16_t enet_uint16;     /**< unsigned 16-bit type */
  typedef uint32_t enet_uint32;      /**< unsigned 32-bit type */
#endif

#endif /* __ENET_TYPES_H__ */

