check_token("http://127.0.0.1:8008/get_user_chats", {method: "GET"}, function(response){
	console.log(response);
	response.json().then(data => {
		console.log(data);
		for (let i = 0; i < data.chats.length; i++)
		{
			document.getElementById("chats-table").innerHTML += "\
				<tr>\
					<td>\
						<img>\
						<div>\
							<a>" + data.chats[i].name + "</a>\
						</div>\
					</td>\
				</tr>";
		}
	});
});