function display_header(title) {
	document.getElementById("header").innerHTML = "\
		<img src=\"/favicon.png\" alt=\"icon\">\
		<div id=\"header-right-block\" class=\"header-right-block\">\
			<div class=\"header-label\"><a>" + title + "</a></div>\
			<div class=\"header-buttons\">\
				<button onclick=\"window.location.assign('/profile')\"><a>Профиль</a></button>\
				<button onclick=\"window.location.assign('/chats')\"><a>Чаты</a></button>\
				<button onclick=\"exit();\"><a>Выйти</a></button>\
			</div>\
		</div>";
}