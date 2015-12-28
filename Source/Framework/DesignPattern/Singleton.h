/************************************************************************/
/* Design Pattern : Singleton
/*
/* ByeDream
/* Dec-25-2015
/************************************************************************/
#ifndef __SINGLETON_H__
#define __SINGLETON_H__

namespace HV
{
	template<class T>
	class Singleton
	{
	public:
		static T *getInstance() {
			if (NULL == mInstance) {
				mInstance = new T;
				mInstance->initialize();
			}
			return mInstance;
		}

		static void destory() {
			SAFE_DELETE(mInstance);
		}

		virtual void initialize() {}
		virtual void finitialize() {}
	protected:
		Singleton() {}
		virtual ~Singleton() {}

		static T *mInstance;
	};

	template<class T>
	T * HV::Singleton<T>::mInstance;
}

#endif // !__SINGLETON_H__

// EOF
