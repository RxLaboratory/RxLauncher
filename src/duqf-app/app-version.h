#ifndef APPVERSION_H
#define APPVERSION_H

#define VERSION_MAJOR 0
#define VERSION_MINOR 0
#define VERSION_BUILD 1
#define VERSION_SUFFIX "Alpha"

#define STRINGIFY_VERSION(A, B, C) CONCAT(A, B, C )
#define CONCAT(A, B, C ) STRINGIFY( A##.##B##.##C )
#define STRINGIFY(A) #A

#define STR_VERSION STRINGIFY_VERSION(VERSION_MAJOR, VERSION_MINOR, VERSION_BUILD) "-" VERSION_SUFFIX

#define VER_FILEVERSION VERSION_MAJOR,VERSION_MINOR,VERSION_BUILD,0
#define STR_FILEVERSION STR_VERSION

// Keep the product version as fixed
#define VER_PRODUCTVERSION VERSION_MAJOR,VERSION_MINOR,VERSION_BUILD,0
#define STR_PRODUCTVERSION STR_VERSION

#define STR_COMPANYNAME "Duduf"
#define STR_FILEDESCRIPTION "DuLauncher"
#define STR_INTERNALNAME "DuLauncher"
#define STR_LEGALCOPYRIGHT "Copyright (c) 2020 Nicolas Dufresne and contributors"
#define STR_LEGALTRADEMARKS1 "All Rights Reserved"
#define STR_PRODUCTNAME "DuLauncher - A Launcher"

#define STR_COMPANYDOMAIN "duduf.com"

#define URL_CHAT ""
#define URL_FORUM ""
#define URL_DOC ""
#define URL_BUGREPORT ""

#define APP_ICON ":/icons/app"

#endif // APPVERSION_H
