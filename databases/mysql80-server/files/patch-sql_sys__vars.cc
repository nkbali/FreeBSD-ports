--- sql/sys_vars.cc.orig	2019-09-20 08:30:51 UTC
+++ sql/sys_vars.cc
@@ -1841,7 +1841,7 @@ static Sys_var_ulong Sys_connect_timeout(
     "The number of seconds the mysqld server is waiting for a connect "
     "packet before responding with 'Bad handshake'",
     GLOBAL_VAR(connect_timeout), CMD_LINE(REQUIRED_ARG),
-    VALID_RANGE(2, LONG_TIMEOUT), DEFAULT(CONNECT_TIMEOUT), BLOCK_SIZE(1));
+    VALID_RANGE(2, INT_MAX32 / 1000), DEFAULT(CONNECT_TIMEOUT), BLOCK_SIZE(1));
 
 static Sys_var_ulong Sys_information_schema_stats_expiry(
     "information_schema_stats_expiry",
@@ -2890,7 +2890,7 @@ static Sys_var_ulong Sys_net_read_timeout(
     "Number of seconds to wait for more data from a connection before "
     "aborting the read",
     SESSION_VAR(net_read_timeout), CMD_LINE(REQUIRED_ARG),
-    VALID_RANGE(1, LONG_TIMEOUT), DEFAULT(NET_READ_TIMEOUT), BLOCK_SIZE(1),
+    VALID_RANGE(1, INT_MAX32 / 1000), DEFAULT(NET_READ_TIMEOUT), BLOCK_SIZE(1),
     NO_MUTEX_GUARD, NOT_IN_BINLOG, ON_CHECK(0),
     ON_UPDATE(fix_net_read_timeout));
 
@@ -2911,7 +2911,7 @@ static Sys_var_ulong Sys_net_write_timeout(
     "Number of seconds to wait for a block to be written to a connection "
     "before aborting the write",
     SESSION_VAR(net_write_timeout), CMD_LINE(REQUIRED_ARG),
-    VALID_RANGE(1, LONG_TIMEOUT), DEFAULT(NET_WRITE_TIMEOUT), BLOCK_SIZE(1),
+    VALID_RANGE(1, INT_MAX32 / 1000), DEFAULT(NET_WRITE_TIMEOUT), BLOCK_SIZE(1),
     NO_MUTEX_GUARD, NOT_IN_BINLOG, ON_CHECK(0),
     ON_UPDATE(fix_net_write_timeout));
 
@@ -4416,7 +4416,7 @@ static Sys_var_ulong Sys_max_execution_time(
     "milliseconds",
     HINT_UPDATEABLE SESSION_VAR(max_execution_time), CMD_LINE(REQUIRED_ARG),
     VALID_RANGE(0, ULONG_MAX), DEFAULT(0), BLOCK_SIZE(1));
-
+#ifndef LIBRESSL_VERSION_NUMBER
 static bool update_fips_mode(sys_var *, THD *, enum_var_type) {
   char ssl_err_string[OPENSSL_ERROR_LENGTH] = {'\0'};
   if (set_fips_mode(opt_ssl_fips_mode, ssl_err_string) != 1) {
@@ -4436,6 +4436,16 @@ static Sys_var_enum Sys_ssl_fips_mode(
     GLOBAL_VAR(opt_ssl_fips_mode), CMD_LINE(REQUIRED_ARG, OPT_SSL_FIPS_MODE),
     ssl_fips_mode_names, DEFAULT(0), NO_MUTEX_GUARD, NOT_IN_BINLOG,
     ON_CHECK(NULL), ON_UPDATE(update_fips_mode), NULL);
+#else
+static const char *ssl_fips_mode_names[] = {"OFF", 0};
+static Sys_var_enum Sys_ssl_fips_mode(
+    "ssl_fips_mode",
+    "SSL FIPS mode (applies only for OpenSSL); "
+    "permitted values are: OFF",
+    GLOBAL_VAR(opt_ssl_fips_mode), CMD_LINE(REQUIRED_ARG, OPT_SSL_FIPS_MODE),
+    ssl_fips_mode_names, DEFAULT(0), NO_MUTEX_GUARD, NOT_IN_BINLOG,
+    ON_CHECK(NULL), ON_UPDATE(NULL), NULL);
+#endif
 
 #if defined(HAVE_OPENSSL)
 static Sys_var_bool Sys_auto_generate_certs(
