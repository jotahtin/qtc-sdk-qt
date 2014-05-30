requires(qtHaveModule(network))

TEMPLATE = subdirs

SUBDIRS += cloudservices

# # Everything else depends on multimedia
# src_qgsttools.subdir = gsttools
# src_qgsttools.depends = multimedia
#
# src_qtmmwidgets.subdir = multimediawidgets
# src_qtmmwidgets.depends = multimedia
# 
# src_plugins.subdir = plugins
# src_plugins.depends = multimedia

qtHaveModule(quick) {
#    src_qtmultimediaquicktools.subdir = qtmultimediaquicktools
#    src_qtmultimediaquicktools.depends = multimedia

    src_imports.subdir = imports
    src_imports.depends = multimedia src_qtmultimediaquicktools

#    SUBDIRS += \
#        src_qtmultimediaquicktools \
#        src_imports
}
