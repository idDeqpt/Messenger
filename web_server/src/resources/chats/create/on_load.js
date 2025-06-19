const params = new URLSearchParams(document.location.search);

let chat_type = params.get("type");

if (chat_type == "user")
{
	document.getElementById("main-chatname").innerHTML = "Имя пользователя";
	document.getElementById("main-create-button").setAttribute("onclick", "create_chat(\"user\", document.getElementById(\"main-chatname-input\").value)");
}
else if (chat_type == "group")
{
	document.getElementById("main-chatname").innerHTML = "Название группы";
	document.getElementById("main-create-button").setAttribute("onclick", "create_chat(\"group\", document.getElementById(\"main-chatname-input\").value)");
}