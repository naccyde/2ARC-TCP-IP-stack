import os
import json
import html
from assets import *

# Create directories
def create_directories(headers):
	if not os.path.exists("manual"):
    		os.makedirs("manual")
	
	for header in headers:
		if not os.path.exists("manual/"+header):
    			os.makedirs("manual/"+header)

# Create index.html file
def create_index_file(headers):
	level = 1
	
	with open("index.html", "w") as i:
		i.write(html.get_html_head(level))
		i.write(html.get_header(level, "", ""))
		i.write(html.get_menu(level, headers))
		i.write(html.get_html_tail())

# Create header file
def create_header_file(name, headers):
	level = 2
	content = ""
	
	with open("data/"+name+".json", "r") as hd:
		data = json.load(hd)
		
		# Create header file
		with open("manual/"+name+".html", "w") as h:
			h.write(html.get_html_head(level))
			h.write(html.get_header(level, name, ""))
			h.write(html.get_menu(level, headers, name, data["functions"]))
			h.write(html.get_html_tail())
		
		for function in data["functions"]:
			create_function_file(name, headers, data["functions"], function["name"], function["return_type"], function["parameters"], function["desc"], function["returns"])
		
# Create functions files
def create_function_file(curr_header, headers, functions, name, return_type, parameters, desc, returns):
	level = 3
	
	with open("manual/"+curr_header+"/"+name+".html", "w") as f:
		f.write(html.get_html_head(level))
		f.write(html.get_header(level, curr_header, name))
		f.write(html.get_menu(level, headers, curr_header, functions, name))
		f.write(html.get_function_content(name, return_type, parameters, desc, returns))		
		f.write(html.get_html_tail())		
