#define DUCKDB_EXTENSION_MAIN

#include "motherduck-extension.hpp"

#include "duckdb.hpp"
#include "duckdb/parser/parsed_data/create_table_function_info.hpp"
#include "s3fs.hpp"

namespace duckdb {

static void LoadInternal(DatabaseInstance &instance) {
	S3FileSystem::Verify(); // run some tests to see if all the hashes work out
	auto &fs = instance.GetFileSystem();

	fs.RegisterSubSystem(make_unique<HTTPFileSystem>());
	fs.RegisterSubSystem(make_unique<S3FileSystem>(BufferManager::GetBufferManager(instance)));

	auto &config = DBConfig::GetConfig(instance);

	// Global HTTP config
	// Single timeout value is used for all 4 types of timeouts, we could split it into 4 if users need that
	config.AddExtensionOption("httpfs_timeout", "HTTP timeout read/write/connection/retry (default 30000ms)",
	                          LogicalType::UBIGINT);
	config.AddExtensionOption("s3_region", "us-east-1", LogicalType::VARCHAR);
	config.AddExtensionOption("s3_access_key_id", instance.config.motherduck_username, LogicalType::VARCHAR);
	config.AddExtensionOption("s3_secret_access_key", instance.config.motherduck_password, LogicalType::VARCHAR);
}

void MotherDuckExtension::Load(DuckDB &db) {
	std::printf(" inside motherduck extension Load ");
	// support only s3 parquet query for now
	LoadInternal(*db.instance);
}
std::string MotherDuckExtension::Name() {
	return "motherduck";
}

} // namespace duckdb

extern "C" {

DUCKDB_EXTENSION_API void motherduck_init(duckdb::DatabaseInstance &db) {
	LoadInternal(db);
}

DUCKDB_EXTENSION_API const char *motherduck_version() {
	return duckdb::DuckDB::LibraryVersion();
}
}

#ifndef DUCKDB_EXTENSION_MAIN
#error DUCKDB_EXTENSION_MAIN not defined
#endif
