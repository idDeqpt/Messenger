var last_message_id = -1;
var users;

async function messages_handler()
{
	const url_params = new URLSearchParams(document.location.search);
	let params = "?chat_id=" + url_params.get("id") + "&message_id=" + last_message_id + "&count=all";
	await check_token("http://" + api_host + "/get_messages_after" + params	, {method: "GET"}, async function(response){
		let data = await response.json();
		if (data.messages == null) return;

		let messages_table = document.getElementById("messages-table");

		let can_scroll = false;
		if ((messages_table.scrollTop + messages_table.clientHeight + 5) >= messages_table.scrollHeight)
			can_scroll = true;

		for (let i = 0; i < data.messages.length; i++)
		{
			let user = users.find(obj => (obj.id == data.messages[i].user_id));
			messages_table.innerHTML += "\
				<div class=\"main-message\">\
					<div class=\"main-message-photo\"><img alt=\"Profile photo\" src=\"data:img/png;base64, " + user.profile_photo_64 + "\"></div>\
					<div class=\"main-message-text\">\
						<div class=\"main-message-text-username\">\
							<a class=\"main-message-text-username-value\" href=\"/profile?id=" + user.id + "\">" + user.username + "</a>\
						</div>\
						<div class=\"main-message-text-content\">\
							<a>" + data.messages[i].text + "</a>\
						</div>\
					</div>\
					<br>\
				</div>";
		}
		last_message_id = data.messages[data.messages.length - 1].id;
		if (can_scroll)
  			messages_table.scrollTop = messages_table.scrollHeight;
	});
}

check_token("http://" + api_host + "/get_chat_data" + document.location.search, {method: "GET"}, async function(response){
	const data = await response.json();

	users = data.members;
	await messages_handler();
	setInterval(async ()=>{await messages_handler();}, 2000);

	document.getElementById("main-header-chatname").innerHTML += data.name;
	if (data.type == "group")
		document.getElementById("main-header-params").setAttribute("onclick", "window.location.assign('/chat/params" + document.location.search + "');");
});