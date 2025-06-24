function change_profile_photo(file)
{
    let reader = new FileReader();
    reader.readAsDataURL(file);
    reader.onload = () => {
    	console.log(reader.result);
    	console.log(reader.result.slice(reader.result.indexOf(",") + 1));
    	send_body = {
			photo_64: reader.result.slice(reader.result.indexOf(",") + 1)
		};
		check_token("http://127.0.0.1:8008/change_profile_photo", {method: "POST", body: JSON.stringify(send_body)}, function(response){
			console.log(response.status);
			if (response.status == 200)
				document.getElementById("main-photo-img").setAttribute("src", "data:img/png;base64, " + send_body.photo_64);
		});
    }
}