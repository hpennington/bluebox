import {MDXProvider} from '@mdx-js/react'
import type {AppProps} from 'next/app'
import '../styles/globals.css'

function App({ Component, pageProps }: AppProps) {
  return (
    <MDXProvider>
      <Component {...pageProps} />
    </MDXProvider>
  )
}

export default App
