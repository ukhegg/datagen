//
// Created by ukheg on 07.04.2017.
//

#ifndef DATAGEN_RANDOM_HPP
#define DATAGEN_RANDOM_HPP


namespace datagen {
	/* 
	 * value of type TValue can be constructed if one of the following createrias are satisifed:
	 * 1. function template<class TInjector> TValue get_random(TInjector&, TValue*) is defined
	 * 2. value is constructible via {}-notation where all parameters are constructible
	 * 3. value is constructible vai ctor, where all parameters are constructible
	 * 
	 * the first met criteria is used to construct random value
	 */

	template<class TValue>
	TValue random()
	{
		
	}
}
#endif //DATAGEN_RANDOM_HPP
