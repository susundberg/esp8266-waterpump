
function make_json_request( url, success_cb )
{
    
    function reqListener () {
        if ( this.status != 200 )
        {
            alert("Failed to fetch: '" + url + "' - got " + this.status );
            return;
        }
        success_cb( JSON.parse( this.responseText ) );
    }

    var request = new XMLHttpRequest();
    request.addEventListener("load", reqListener);
    request.open("GET", url );
    request.send();

}

