check_token("http://127.0.0.1:8008/get_chat_data" + document.location.search, {method: "GET"}, function(response){
	console.log(response);
	response.json().then(data => {
		console.log(data);
		document.getElementById("main-header-chatname").innerHTML += data.chat_name;
		for (let i = 0; i < data.messages.length; i++)
		{
			document.getElementById("messages-table").innerHTML += "\
				<div>\
					<div>\
						<a>" + data.chat_members.find(obj => (obj.id == data.messages[i].user_id)).username + "</a>\
					</div>\
					<div>\
						<a>" + data.messages[i].text + "</a>\
					</div>\
					<br>\
				</div>";
		}
	});
});