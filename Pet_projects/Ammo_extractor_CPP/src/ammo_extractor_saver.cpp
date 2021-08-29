#include <ammo_extractor_saver.h>

int ammoExtractor::callback(void *NotUsed, int argc, char **argv, char **azColName) {

    for(int i = 0; i < argc; i++){
        
        printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }
    
    printf("\n");
    return 0;
}

auto ammoExtractor::getUnixTime() {

    auto now = std::chrono::system_clock::now();
    return std::to_string(std::chrono::system_clock::to_time_t(now));
}

auto ammoExtractor::getRawJsonStrings() {
    
    std::ifstream fin;
    
    fin.open(JSONpath);

    try{

        if(fin.is_open()) {

            while(!fin.eof()) {
                    
                getline(fin, dummy);                        
                if(!dummy.empty()) {

                    dummy.shrink_to_fit();
                    buffer.append(std::move(dummy));
                }
            } 
            
            fin.close();          
        }
        else {
            
            std::cerr << "Не удается прочитать JSON-файл..." << std::endl;
            return errCode::CANT_READ_JSON;
        }
    }
    catch(...) {

        fin.close();
        std::cerr << "Не удается прочитать JSON-файл..." << std::endl;
        return errCode::CANT_READ_JSON;
    }
    
    return errCode::DONE;
}

auto ammoExtractor::parseJSONStrings() {

    Json::Value root(buffer);
    JSONCPP_STRING err;

    Json::CharReaderBuilder builder;

    const std::unique_ptr<Json::CharReader> reader(builder.newCharReader());

    if (!reader->parse(buffer.c_str(), buffer.c_str() + buffer.length(), &root, &err)) {
      
        std::cerr << "Ошибка в обработке JSON-файла(1)..." << std::endl;
        return errCode::CANT_PROCESS_JSON;     
    }    

    try{

        const Json::Value vendors = root["Vendors"];

        for(int i = 0; i < vendors.size(); ++i) {

            ss.str(std::string());
            ss << vendors[i];

            auto arg = std::move(ss.str());

            Json::Value root_(arg);
            JSONCPP_STRING err_;

            Json::CharReaderBuilder builder_;

            const std::unique_ptr<Json::CharReader> reader(builder_.newCharReader());

            if (!reader->parse(arg.c_str(), arg.c_str() + arg.length(), &root_, &err_)) {
              
                std::cerr << "Ошибка в обработке JSON-файла(2)..." << std::endl;
                return errCode::CANT_PROCESS_JSON;     
            }

            const Json::Value ammo = root_["Ammunitions"];

            for(int j = 0; j < ammo.size(); ++j) {

                bulletDataUnit.ItemID = ammo[j]["ItemID"].asUInt64();
                bulletDataUnit.ItemGUID = ammo[j]["ItemGUID"].asString();
                bulletDataUnit.ItemModifiedWhen = ammo[j]["ItemModifiedWhen"].asString();
                bulletDataUnit.Vendor = ammo[j]["Vendor"].asString();
                bulletDataUnit.Caliber = ammo[j]["Caliber"].asString();
                bulletDataUnit.GrammWeight = std::floor(ammo[j]["GrammWeight"].asDouble()*10000)/10000;
                bulletDataUnit.BallisticCoefficient = ammo[j]["BallisticCoefficient"].asDouble();
                bulletDataUnit.BC_G7 = std::floor(bulletDataUnit.BallisticCoefficient*0.502*1000)/1000;
                bulletDataUnit.MuzzleSpeed = static_cast<uint16_t>(ammo[j]["MuzzleSpeed"].asUInt());
                bulletDataUnit.GrainWeight = static_cast<uint16_t>(ammo[j]["GrainWeight"].asUInt());
                bulletDataUnit.BulletShape = ammo[j]["BulletShape"].asString();

                allAmmos.push_back(std::move(bulletDataUnit));
            }
        }
    }
    catch(std::exception& ex) {
    
        std::cerr << "Ошибка в парсинге JSON-файла..." << std::endl;
        return errCode::CANT_PARSE_JSON;
    }

#if defined DEBUG
    std::cout << "Вывод результатов парсинга (для " << allAmmos.size() << " патронов)" << std::endl;

    for(const auto& i : allAmmos) {

        std::cout << "ID: " << i.ItemID << std::endl;
        std::cout << "GUID: " << i.ItemGUID << std::endl;
        std::cout << "Дата изменения: " << i.ItemModifiedWhen << std::endl;
        std::cout << "Производитель: " << i.Vendor << std::endl;
        std::cout << "Калибр (тип): " << i.Caliber << std::endl;
        std::cout << "Вес (грамм): " << i.GrammWeight << std::endl;
        std::cout << "Вес (гран): " << i.GrainWeight << std::endl;
        std::cout << "БК по G1: " << i.BallisticCoefficient << std::endl;
        std::cout << "БК по G7: " << i.BC_G7 << std::endl;
        std::cout << "Дульная скорость (м/с): " << i.MuzzleSpeed << std::endl;
        std::cout << "Форма пули (тип): " << i.BulletShape << "\n" << std::endl;
    }
#endif
    return errCode::DONE;
}

auto ammoExtractor::saveToSqlite() {

    std::string DBpath{"bullet_DB_type1_"};
    DBpath.append(getUnixTime());
    DBpath.append(".sqlite");

    auto rc = sqlite3_open(DBpath.c_str(), &db);
    
    if(rc!=SQLITE_OK) {
        
        std::cerr << "Не могу открыть базу данных SQLite: " << DBpath << std::endl;
        std::cerr << "Ошибка: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        return errCode::NO_SQLITE_DB;
    }

    const char* dropTableRequest = "DROP TABLE IF EXISTS bullets_info;";

    rc = sqlite3_exec(db, dropTableRequest, callback, 0, &zErrMsg);

    if( rc!=SQLITE_OK ) {
        
        std::cerr << "Ошибка SQL: " << zErrMsg << std::endl;
        sqlite3_free(zErrMsg);
        sqlite3_close(db);
        return errCode::CANT_DROP_TABLE;
    }
   
    const char* createTableRequest = "CREATE TABLE bullets_info (\
        id_abs INTEGER PRIMARY KEY,\
        bullet_ID INTEGER,\
        bullet_GUID TEXT,\
        data_modified TEXT,\
        vendor_name TEXT,\
        caliber_type TEXT,\
        weight_gramm REAL,\
        weight_grain INTEGER,\
        BC_G1 REAL,\
        BC_G7 REAL,\
        muzzle_speed INTEGER,\
        bullet_shape TEXT);";

    rc = sqlite3_exec(db, createTableRequest, callback, 0, &zErrMsg);

    if( rc!=SQLITE_OK ) {
        
        std::cerr << "Ошибка SQL: " << zErrMsg << std::endl;
        sqlite3_free(zErrMsg);
        sqlite3_close(db);
        return errCode::CANT_CREATE_TABLE;
    }

    for(const auto& i : allAmmos) {
   
        ss.str(std::string());
        
        ss << "INSERT INTO bullets_info (bullet_ID, bullet_GUID,"
        " data_modified, vendor_name, caliber_type, weight_gramm," 
        " weight_grain, BC_G1, BC_G7, muzzle_speed, bullet_shape) VALUES (" 
        << i.ItemID << ", '" << i.ItemGUID << "', '" << i.ItemModifiedWhen << "', '"
        << i.Vendor << "', '" << i.Caliber << "', " << i.GrammWeight << ", "
        << i.GrainWeight << ", " << i.BallisticCoefficient << ", " 
        << i.BC_G7 << ", " << i.MuzzleSpeed << ", '" << i.BulletShape << "');";

        rc = sqlite3_exec(db, ss.str().c_str(), callback, 0, &zErrMsg);
        
        if( rc!=SQLITE_OK ) {

            std::cerr << "Ошибка SQL: " << zErrMsg << std::endl;
            sqlite3_free(zErrMsg);
            sqlite3_close(db);
            return errCode::CANT_INSERT_INFO;
        }
    }
    
    sqlite3_close(db);   
    return errCode::DONE;
}

ammo_error_t ammoExtractor::parseAndStore() {

    ammo_error_t result{errCode::DONE};

    if((result = getRawJsonStrings()) != errCode::DONE) {

        return result;
    }

    if((result = parseJSONStrings()) != errCode::DONE) {

        return result;
    }

    result = saveToSqlite();

    return result;
}