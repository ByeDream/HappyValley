/************************************************************************/
/* Design Pattern : Factory
/*
/* ByeDream
/* Dec-25-2015
/************************************************************************/
#ifndef __FACTORY_H__
#define __FACTORY_H__

namespace HV
{
	namespace FACTORY
	{
		class Product
		{
		public:
			virtual ~Product() {}
		};

		class Worker
		{
		public:
			virtual ~Worker() {}
			virtual Product *doWork() const = 0;
		};

		class Factory
		{
			typedef std::map<INDEX_T, const Worker *> WorkerGroup;
		public:
			virtual void RegisterWorker(INDEX_T id, const Worker *worker)
			{
				mWorkerGroup[id] = worker;
			}
			virtual void UnregisterWorker(INDEX_T id)
			{
				WorkerGroup::iterator itor = mWorkerGroup.find(id);
				if (itor != mWorkerGroup.end())
					mWorkerGroup.erase(itor);
			}
			virtual Product *Produce(INDEX_T id)
			{
				Product *product = NULL;
				WorkerGroup::iterator itor = mWorkerGroup.find(id);
				if (itor != mWorkerGroup.end())
					product = itor->second->doWork();
				return product;
			}

		protected:
			WorkerGroup mWorkerGroup;
		};

		template<class W>
		class WorkerRegisterer
		{
		public:
			WorkerRegisterer(Factory *factory, INDEX_T id)
			{
				factory->RegisterWorker(id, new W);
			}
		};

#		define REGISTER_WORKER(workerClass, id, pFactory) \
		WorkerRegisterer<workerClass> __reg((pFactory), (id));

		template<class P>
		class AutoWorker : public Worker
		{
		public:
			virtual Product *doWork() const { return dynamic_cast<Product *>(new P); }
		};

#		define REGISTER_AUTO_WORKER(productClass, id, pFactory) \
		WorkerRegisterer<HV::FACTORY::AutoWorker<productClass>> __reg((pFactory), (id));
	}
}


#endif // !__FACTORY_H__