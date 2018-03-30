#!/bin/usr/python3
#-*-coding:utf-8*-

import files
from assets import headers

files.create_directories(headers)
files.create_index_file(headers)

for header in headers:
	print("Generating : " + header)
	files.create_header_file(header, headers)
