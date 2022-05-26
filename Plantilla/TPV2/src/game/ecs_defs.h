// This file is part of the course TPV2@UCM - Samir Genaim


#pragma once

// Components list - must have at least one element
//
#define _CMPS_LIST_ \
	_TRANSFORM, \
	_IMAGE, \
	_FIGHTERCTRL, \
	_STOPONBORDER, \
    _TOROIDE, \
    _DEACC, \
    _HEALTH, \
    _GUN, \
	_FRAMEDIMAGE, \
	_GENERATIONS, \
	_ASTEROIDTYPE

// Groups list - must have at least one element
//
#define _GRPS_LIST_ \
	_grp_ASTEROIDS, \
    _grp_BULLETS, \
    _grp_LIVES

// Handlers list - must have at least one element
//
#define _HDLRS_LIST_ \
	_hdlr_CAZA

// Systems list - must have at least one element
//
#define _SYS_LIST_ \
	_sys_RENDERSYSTEM, \
    _sys_COLLISIONSYSTEM, \
    _sys_FIGHTERSYSTEM, \
    _sys_BULLETSYSTEM, \
    _sys_ASTEROIDSYSTEM, \
    _sys_GAMECTRLSYSTEM

