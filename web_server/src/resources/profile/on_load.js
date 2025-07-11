const params = new URLSearchParams(document.location.search);
let token_payload = localStorage.getItem("access_token").split(".")[1];
let self_page = params.get("id") == null;
let user_id = (self_page) ? JSON.parse(atob(token_payload))["id"] : params.get("id");
if (self_page)
	document.getElementById("main-header").innerHTML += "<div class=\"main-header-buttons\"><button class=\"custom-button\" onclick=\"window.location.assign('/profile/params' + document.location.search);\"><a>Параметры</a></button></div>";

check_token("http://" + api_host + "/get_profile_data?id=" + user_id, {method: "GET"}, function(response){
	response.json().then(data => {
		console.log(data);
		document.getElementById("main-header-info-photo").setAttribute("src", "data:img/png;base64, " + data.profile_photo_64);
		document.getElementById("main-header-info-username").innerHTML = data.username;
		document.getElementById("main-header-info-description").innerHTML = (data.description == "") ? "No description" : data.description;
	});
});