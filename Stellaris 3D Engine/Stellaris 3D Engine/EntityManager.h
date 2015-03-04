//====================================================================================
// FILE:    EntityManager.h ----------------------------------------------------------
//------------------------------------------------------------------------------------

#ifndef _ENTITY_MANAGER_H_
#define _ENTITY_MANAGER_H_


#include <unordered_map>


namespace sge
{
	template <class TCKey, class TCVal>

	class CEntityManager
	{
	public:
		CEntityManager();
		~CEntityManager();



	private:
		// The next ID which will be used as a key for the next entity that is placed into
		// the hash map
		mNextEID;

	};
}


#endif /* _ENTITY_MANAGER_H_ */
