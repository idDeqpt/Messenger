function change_profile_photo(file)
{
	document.getElementById("main-data-photo").innerHTML += "<a id=\"main-data-photo-warning\">Загрузка изображения</a>";
    let reader = new FileReader();
    reader.readAsDataURL(file);
    let send_body;
    reader.onload = () => {
    	const img = new Image();
        img.src = reader.result;
        img.onload = function () {
        	let size = 256;
            const canvas = document.createElement('canvas');
            canvas.width = size;
            canvas.height = size;

            const ctx = canvas.getContext('2d');
            ctx.drawImage(img, 0, 0, canvas.width, canvas.height);

            const newImageUrl = canvas.toDataURL('image/png');
            send_body = {
				photo_64: newImageUrl.slice(newImageUrl.indexOf(",") + 1)
			};

			check_token("http://" + api_host + "/change_profile_photo", {method: "POST", body: JSON.stringify(send_body)}, function(response){
				console.log(response.status);
				if (response.status == 200)
				{
					document.getElementById("main-data-photo-current").setAttribute("src", "data:img/png;base64, " + send_body.photo_64);
					document.getElementById("main-data-photo-warning").remove();
				}
				else
				{
					document.getElementById("main-data-photo-warning").setAttribute("class", "warning");
					document.getElementById("main-data-photo-warning").innerHTML = "Ошибка загрузки";
				}
			});
        };
    }
}