TEMPLATE = subdirs
SUBDIRS = shared client server
server.depends = shared
client.depends = shared