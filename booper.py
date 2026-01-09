import requests

def main():
    local_url: str = "http://localhost:9002"
    # GET request
    get_res = requests.get(local_url)
    print(f'GET Response: {get_res.text}')

    # POST request
    post_res = requests.post(local_url, data={'key': 'value'})
    print(f'POST Response: {post_res.text}')

if __name__ == "__main__":
    main()