import nextMDX from '@next/mdx'

const withMDX = nextMDX({
  // By default only the .mdx extension is supported.
  extension: /\.mdx?$/,
  options: {providerImportSource: '@mdx-js/react',  /* otherOptions… */},
  images: {
    domains: ['github.com'],
  },
})

export default withMDX({
  // Support MDX files as pages:
  pageExtensions: ['md', 'mdx', 'tsx', 'ts', 'jsx', 'js'],
  reactStrictMode: true,
  swcMinify: true,
})

