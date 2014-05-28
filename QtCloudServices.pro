# iOS does not require OpenSSL and always passes
!ios:requires(contains(QT_CONFIG, openssl) | contains(QT_CONFIG, openssl-linked))

lessThan(QT_MAJOR_VERSION, 5): error("The Enginio Qt library only supports Qt 5.")
load(configure)
load(qt_parts)

TEMPLATE = subdirs

SUBDIRS += \
    QtCloudServices \
    examples \
    Applications


