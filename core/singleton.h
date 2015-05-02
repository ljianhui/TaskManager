#ifndef _SINGLETON_H
#define _SINGLETON_H

#include <stdlib.h>

template<typename T>
class Singleton
{
	public:
		static T* getInstance()
		{
			if (sInstance == NULL)
			{
				sInstance = new T;
			}
			++sReference;
			return sInstance;
		}

		void releaseInstance()
		{
			--sReference;
			if (sReference == 0)
			{
				delete sInstance;
				sInstance = NULL;
			}
		}

	protected:
		Singleton(){}
		virtual ~Singleton(){}

	private:
		static T* sInstance;
		static int sReference;
};

template<typename T>
T* Singleton<T>::sInstance(NULL);
template<typename T>
int Singleton<T>::sReference(0);

#endif

