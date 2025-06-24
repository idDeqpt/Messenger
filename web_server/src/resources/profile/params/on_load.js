const params = new URLSearchParams(document.location.search);
let token_payload = localStorage.getItem("access_token").split(".")[1];
let user_id = (params.get("id") == null) ? JSON.parse(atob(token_payload))["id"] : params.get("id");

check_token("http://127.0.0.1:8008/get_profile_data?id=" + user_id, {method: "GET"}, function(response){
	response.json().then(data => {
		console.log(data);
		document.getElementById("main-photo-current").setAttribute("src", "data:img/png;base64, " + data.profile_photo_64);
		document.getElementById("main-username-current").innerHTML = data.username;
		document.getElementById("main-description-current").innerHTML = (data.description == "") ? "No description" : data.description;
		document.getElementById("main-photo-warning").remove();
	});
});