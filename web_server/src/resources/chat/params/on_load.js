check_token("http://" + api_host + "/get_chat_data" + document.location.search, {method: "GET"}, async function(response){
	const data = await response.json();
	console.log(data);
	for (let i = 0; i < data.members.length; i++)
		document.getElementById("chat-members").innerHTML += "<a>" + data.members[i].username + "</a>";
});