import logo from "./logo.svg";
import "./App.css";
import Footer from "./components/footer";
import { useState } from "react";

function App() {
  const [selectedFile, setSelectedFile] = useState(null);

  // On file select (from the pop up)
  const onFileChange = (event) => {
    // Update the state
    setSelectedFile(event.target.files[0]);
  };

  // On file upload (click the upload button)
  const onFileUpload = () => {
    const formData = new FormData();
    formData.append("myFile", selectedFile, selectedFile.name);

    console.log(selectedFile);

    // Request made to the backend api
    // Send formData object
    // axios.post("api/uploadfile", formData);
  };

  // File content to be displayed after
  // file upload is complete
  const fileData = () => {
    if (selectedFile) {
      return (
        <div className="m-4">
          <h2>File Details:</h2>
          <p>File Name: {selectedFile.name}</p>
          <p>File Type: {selectedFile.type}</p>
          <p>
            Last Modified:
            {selectedFile.lastModifiedDate.toDateString()}
          </p>
        </div>
      );
    } else {
      return (
        <div>
          <br />
          <h4>Choose before Pressing the Upload button</h4>
        </div>
      );
    }
  };
  return (
    <div className="page-container">
      <main className="container content-wrapper">
        <h1>RISC-V Disassembler</h1>
        <div className="m-4">
          <input type="file" onChange={onFileChange} />
          <button onClick={onFileUpload}>Upload!</button>
        </div>
        {fileData()}
      </main>
      {/* <Footer /> */}
    </div>
  );
}

export default App;
