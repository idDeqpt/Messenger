async function add_member()
{
	const params = new URLSearchParams(document.location.search);
	let send_body = {
		chat_id: Number(params.get("id")),
		username: document.getElementById("username").value
	};
	await check_token("http://127.0.0.1:8008/add_member_to_chat",
		{method: "POST", body: JSON.stringify(send_body)},
		async function(response){
			if (response.status == 404)
				document.getElementById("warning-field").innerHTML = "Пользователь не найден";
			else if (response.status == 403)
				document.getElementById("warning-field").innerHTML = "Недостаточно прав";
			else if (response.status == 409)
				document.getElementById("warning-field").innerHTML = "Пользователь уже добавлен";
			else
				document.getElementById("warning-field").innerHTML = "Успешно";
	});
}