TEMPLATE = subdirs

qtHaveModule(widgets) {
    SUBDIRS += Widgets
#     qtHaveModule(quick) {
#       SUBDIRS += quick
#   }
}
