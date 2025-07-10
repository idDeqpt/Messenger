function display_header(title) {
	document.getElementById("header").innerHTML = "\
		<button onclick=\"window.location.assign('/profile')\"><a>Профиль</a></button>\
		<button onclick=\"window.location.assign('/chats')\"><a>Чаты</a></button>\
		<button onclick=\"exit();\"><a>Выйти</a></button>\
	";
}