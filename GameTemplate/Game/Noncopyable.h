#pragma once
/*!
 * @brief	Noncopy�|���V�[
 */

#ifndef _TKNONCOPYABLE_H_
#define _TKNONCOPYABLE_H_

//�I�u�W�F�N�g���R�s�[���ł��Ȃ��悤�ɂ���B
struct Noncopyable {
	Noncopyable() = default;
	Noncopyable(const Noncopyable&) = delete;
	Noncopyable& operator=(const Noncopyable&) = delete;
};


#endif // _TKNONCOPYABLE_H_