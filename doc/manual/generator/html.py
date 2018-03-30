import assets

def get_main_title(level):
	if (level == 1):
		return assets.html_header_index
	elif (level == 2):
		return assets.html_header_header
	elif (level == 3):
		return assets.html_header_function

def get_header_title(header):
	return '<h2 class="HeaderTitle">'+header+'</h2>'

def get_function_title(function):
	return '<h3 class="FunctionTitle">'+function+'</h3>'

def get_header(level, header, function):
	hdr = '<header class="Header">'
	
	hdr += get_main_title(level)
	
	if (level == 2 or level == 3):
		hdr += get_header_title(header)
	
	if (level == 3):
		hdr += get_function_title(function)
		
	hdr += '</header>'
	
	return hdr
	
def get_html_head(level):
	html_head = assets.html_head_up
	
	if (level == 1):
		html_head += assets.html_css_index
	elif (level == 2):
		html_head += assets.html_css_header
	elif (level == 3):
		html_head += assets.html_css_function
	
	html_head += assets.html_head_down
	return html_head
	
def get_html_tail():
	return assets.html_tail
	
def __get_main_menu(headers):
	menu = ""
	
	for header in headers:
		menu += '<li><a href="manual/'+header+'.html">'+header+'.h</a></li>'
	
	return menu
	
def __get_header_menu(headers, curr_header, functions):
	menu = ""
	
	for header in headers:
		if (header == curr_header):
			menu += '<li class="SelectedObject">'+header+'.h</li>'
			menu += '<li><ul>'
			for function in functions:
				menu += '<li><a href="'+header+'/'+function["name"]+'.html">'+function["name"]+'</a></li>'
			menu += '</ul></li>'
		else:
			menu += '<li><a href="'+header+'.html">'+header+'.h</a></li>'

	return menu
		
def __get_function_menu(headers, curr_header, functions, curr_function):
	menu = ""
	
	for header in headers:
		menu += '<li><a href="../'+header+'.html">'+header+'.h</a></li>'
		if (header == curr_header):
			menu += '<li><ul>'
			for function in functions:
				if (function["name"] == curr_function):
					menu += '<li class="SelectedObject">'+function["name"]+'</li>'
				else:
					menu += '<li><a href="'+function["name"]+'.html">'+function["name"]+'</a></li>'
			menu += '</ul></li>'

	return menu

def get_menu(level, headers=[], curr_header="", functions=[], curr_function=""):
	nav = assets.html_nav_head
	
	if (level == 1):
		nav += __get_main_menu(headers)
	elif (level == 2):
		nav += __get_header_menu(headers, curr_header, functions)
	elif (level == 3):
		nav += __get_function_menu(headers, curr_header, functions, curr_function)
	
	nav += assets.html_nav_tail
	
	return nav

def __get_function_proto(return_type, name, parameters):
	proto = '<p>'
	proto += '<span class="ReturnType">'+return_type+'</span>'
	proto += ' '
	proto += '<span class="FunctionName">'+name+'</span>'
	proto += '('
	
	for i, parameter in enumerate(parameters):
		if i != 0:
			proto += ', '
		proto += '<span class="ParameterType">'+parameter["type"]+'</span>'
		proto += ' '
		proto += '<span class="ParameterName">'+parameter["name"]+'</span>'
		
	proto += ')'	
	proto += '</p>'
	
	return proto
	
def get_function_content(name, return_type, parameters, desc, returns):
	ctt = '<div class="ContentDescription">\n'
	ctt += '<hr>'
	ctt += __get_function_proto(return_type, name, parameters)
	ctt += '<p>DESCRIPTION : '+desc+'</p>\n'
	ctt += '<p>PARAMETERS :</p>\n'
		
	for parameter in parameters:
		ctt += '<p class="FunctionParameterInfo">'+parameter["type"]+' '+parameter["name"]+' : '+parameter["desc"]+'</p>\n'
		
	ctt += '<p>RETURN : </p>\n'
	
	for return_type in returns:
		ctt += '<p class="FunctionReturnInfo">'+return_type["type"]+' : '+return_type["reason"]+'</p>\n'
	
	ctt += '<hr>\n\
		</div>\n'
	
	return ctt	
