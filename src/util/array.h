/*
 * array.h
 *
 *  Created on: 5 janv. 2015
 *      Author: jfellus
 */

#ifndef ARRAY_H_
#define ARRAY_H_

#include <pthread.h>
#include <vector>

template <typename T> class array {
	std::vector<T> v;
	pthread_mutex_t mut = PTHREAD_MUTEX_INITIALIZER;
public:
	array() {}
	~array() {}

	inline void add(const T& t) {
		pthread_mutex_lock(&mut);
		v.push_back(t);
		pthread_mutex_unlock(&mut);
	}

	inline void clear() {
		pthread_mutex_lock(&mut);
		v.clear();
		pthread_mutex_unlock(&mut);
	}

	inline const T& operator[](int i) const {	return v[i];	}
};


#endif /* ARRAY_H_ */
