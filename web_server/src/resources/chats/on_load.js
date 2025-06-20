check_token("http://127.0.0.1:8008/get_user_chats", {method: "GET"}, function(response){
	response.json().then(data => {
		for (let i = 0; i < data.chats.length; i++)
		{
			document.getElementById("main-chats").innerHTML += "\
				<div class=\"main-chats-chat\" onclick=\"window.location.assign('/chat?id=" + data.chats[i].id + "');\">\
					<a>" + data.chats[i].name + "</a>\
					<a>" + ((data.chats[i].last_message == null) ? "No messages" : data.chats[i].last_message.sender_username + ": " + data.chats[i].last_message.text) + "</a>\
				</div>";
		}
	});
});