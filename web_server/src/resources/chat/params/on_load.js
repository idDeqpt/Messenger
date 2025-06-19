check_token("http://127.0.0.1:8008/get_chat_data" + document.location.search, {method: "GET"}, async function(response){
	const data = await response.json();
	console.log(data);
	for (let i = 0; i < data.members.length; i++)
		document.getElementById("chat-members").innerHTML += "<a>" + data.members[i].username + "</a>";

	document.getElementById("main-header-chatname").innerHTML += data.name;
	document.getElementById("main-header").innerHTML += (data.type == "group") ? "\
	<div><button onclick=\"window.location.assign('/chat/params" + document.location.search + "');\"><a>Парамтеры</a></button></div>\
	" : "";
});