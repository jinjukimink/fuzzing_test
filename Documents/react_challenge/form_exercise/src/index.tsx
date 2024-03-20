import React from 'react';
import ReactDOM from 'react-dom/client';
import App from './App';
import { darkTheme } from "./theme";
import {RecoilRoot} from "recoil";
import { ThemeProvider } from 'styled-components';

//const root = ReactDOM.createRoot(document.getElementById('root'));
const root = ReactDOM.createRoot(document.getElementById("root") as HTMLElement);

root.render(
  <React.StrictMode>
    <RecoilRoot>
      <ThemeProvider theme={darkTheme}>
    <App />
    </ThemeProvider>
    </RecoilRoot>
  </React.StrictMode>
);


