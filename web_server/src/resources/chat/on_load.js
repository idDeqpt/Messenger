var messages_count = 0;

async function messages_handler(users)
{
	await check_token("http://127.0.0.1:8008/get_chat_messages" + document.location.search, {method: "GET"}, async function(response){
		let data = await response.json();
		if (data.messages == null) return;

		for (let i = messages_count; i < data.messages.length; i++)
			document.getElementById("messages-table").innerHTML += "\
				<div>\
					<div>\
						<a>" + users.find(obj => (obj.id == data.messages[i].user_id)).username + "</a>\
					</div>\
					<div>\
						<a>" + data.messages[i].text + "</a>\
					</div>\
					<br>\
				</div>";
		messages_count = data.messages.length;
	});
}

check_token("http://127.0.0.1:8008/get_chat_data" + document.location.search, {method: "GET"}, async function(response){
	const data = await response.json();

	await messages_handler(data.members);
	setInterval(async ()=>{await messages_handler(data.chat_members);}, 2000);

	document.getElementById("main-header-chatname").innerHTML += data.name;
	document.getElementById("main-header").innerHTML += (data.type == "group") ? "\
	<div><button onclick=\"window.location.assign('/add_member_form');\"><a>Добавить участника</a></button></div>\
	" : "";
});