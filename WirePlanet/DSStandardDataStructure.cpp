#include"DSStandardDataStructure.h"

/*DataScript標準データ構造
DataScriptを用いてデータを定義する際の標準形式。複数形式のデータを混在させることが可能。

//定義されているデータの種類を宣言する
property{
	DataScriptStandardDataStructure,1.0;
	ID, [データ識別ID], [対応データブロック名(省略でIDと同じになる)];
}
[データブロック名]{
	定義
}
……
*/

namespace DSsds{
	namespace{
		const char* DSSDS_PROPERTY_BLOCK_NAME("property");
		const char* DSSDS_NAME("DataScriptStandardDataStructure");

		bool check_dssds(std::shared_ptr<const DataBlock> db){
			const auto& dat = db->GetAllData();
			for (const auto& l : dat){
				if (l.size() != 2){ continue; }
				if (l[0] == DSSDS_NAME){
					return true;
				}
			}
			return false;
		}
		const std::string get_blockname_by_id(std::shared_ptr<const DataBlock> db, const std::string& id){
			const auto& dat = db->GetAllData();
			for (const auto& l : dat){
				if (l.size() < 2){ continue; }
				if (l[0] == "ID"){
					if (l[1] == id){
						return l.size() == 3 ? l[2] : id;
					}
				}
			}
			return not_exist_string;
		}
	}

	const std::string GetDataBlockNameByDataID(const DataScript& ds, const std::string& id)
	{
		try{
			auto db = ds.GetRoot()->FindChild(DSSDS_PROPERTY_BLOCK_NAME);
			if (db == nullptr){ return not_exist_string; }
			if (check_dssds(db) == false){ return not_exist_string; }
			return get_blockname_by_id(db, id);
		}
		catch (std::out_of_range&){
			return not_exist_string;
		}
	}

	std::shared_ptr<const DataBlock> GetDataBlockByDataID(const DataScript& ds, const std::string& id)
	{
		try{
			auto db = ds.GetRoot()->FindChild(DSSDS_PROPERTY_BLOCK_NAME);
			if (db == nullptr){ return nullptr; }
			if (check_dssds(db) == false){ return nullptr; }
			const auto dbn = get_blockname_by_id(db, id);

			return ds.GetRootDataBlock().FindChild(dbn);
		}
		catch (std::out_of_range&){
			return nullptr;
		}
	}

	bool IsValidDataID(const DataScript& ds, const std::string& id)
	{
		try{
			auto db = ds.GetRoot()->FindChild(DSSDS_PROPERTY_BLOCK_NAME);
			if (db == nullptr){ return false; }
			if (check_dssds(db) == false){ return false; }
			const auto dbn = get_blockname_by_id(db, id);
			return ds.GetRootDataBlock().FindChild(dbn) == nullptr ? false : true;
		}
		catch (std::out_of_range&){
			return false;
		}
	}

	bool IsDSSDS(const DataScript& ds)
	{
		try{
			auto db = ds.GetRoot()->FindChild(DSSDS_PROPERTY_BLOCK_NAME);
			if (db == nullptr){ return false; }
			return check_dssds(db);
		}
		catch (std::out_of_range&){
			return false;
		}
	}
}